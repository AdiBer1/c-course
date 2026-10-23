#!/usr/bin/env bash
# score.sh - build, test and score one task, exactly like CI does.
#
#   scripts/score.sh tasks/07-command-parser      (or: make score, inside the task)
#
# Writes   <task>/build/score.json   machine-readable result
#          <task>/build/score.md     readable report (also printed)
#
# Score (0-100):
#   does not compile -> 0
#   tests            -> 70 points, proportional to the share of passing tests
#   code quality     -> 30 points minus deductions (never below 0):
#       compiler warning        -5 each
#       cppcheck finding        -3 each (from stage 5; before that: notes only)
#       sanitizer report        -10 each distinct report
#       forbidden symbol        -15 each symbol used
#       ANSWERS.md left empty   -10
#
# Exit code: 0 when a score was produced (even a low one), 2 on internal error.

set -uo pipefail

PASS_THRESHOLD=60
TEST_POINTS=70
QUALITY_POINTS=30
PTS_WARNING=5
PTS_CPPCHECK=3
PTS_SANITIZER=10
PTS_FORBIDDEN=15
PTS_ANSWERS=10

die() { echo "score.sh: $*" >&2; exit 2; }
neg() { if [ "$1" -gt 0 ]; then echo "−$1"; else echo 0; fi; }

[ $# -ge 1 ] || die "usage: score.sh <task-dir>"
TASK_DIR=$(cd "$1" 2>/dev/null && pwd) || die "no such directory: $1"
[ -f "$TASK_DIR/task.mk" ] || die "$TASK_DIR is not a task (no task.mk)"
cd "$TASK_DIR"

# ---------------------------------------------------------------- config
TASK=""; TASK_STAGE=0; STUDENT_FILES=""; FORBIDDEN=""; CPPCHECK_COUNTS=0; INCLUDES=""
while IFS= read -r line; do
    key=${line%%=*}; val=${line#*=}
    case $key in
        TASK) TASK=$val ;;
        TASK_STAGE) TASK_STAGE=$val ;;
        STUDENT_FILES) STUDENT_FILES=$val ;;
        FORBIDDEN) FORBIDDEN=$val ;;
        CPPCHECK_COUNTS) CPPCHECK_COUNTS=$val ;;
        INCLUDES) INCLUDES=$val ;;
    esac
done < <(make -s --no-print-directory config 2>/dev/null)
[ -n "$TASK" ] || die "could not read task settings (make config failed)"

STUDENT_C=()
for f in $STUDENT_FILES; do
    case $f in *.c) STUDENT_C+=("$f") ;; esac
done

make -s --no-print-directory clean >/dev/null 2>&1
OUT=build
LOG=build/score-logs
mkdir -p "$LOG"

TIMEOUT=()
if command -v timeout >/dev/null 2>&1; then TIMEOUT=(timeout 5s); fi
LEAKS=0; [ "$(uname -s)" = Linux ] && LEAKS=1
export ASAN_OPTIONS="halt_on_error=0:abort_on_error=0:detect_leaks=$LEAKS:detect_stack_use_after_return=1:print_summary=1"
export UBSAN_OPTIONS="halt_on_error=0:print_stacktrace=1"

is_student_file() {
    local f
    for f in $STUDENT_FILES; do [ "$f" = "$1" ] && return 0; done
    return 1
}

# ---------------------------------------------------------------- build
make --no-print-directory all >"$LOG/build.txt" 2>&1
BUILD_RC=$?
COMPILED=true; [ $BUILD_RC -eq 0 ] || COMPILED=false

# Compiler warnings in the student's own files (distinct lines).
: >"$LOG/warnings.txt"
grep -E '^[^ :]+:[0-9]+:[0-9]+: warning: ' "$LOG/build.txt" 2>/dev/null | sort -u |
while IFS= read -r w; do
    if is_student_file "${w%%:*}"; then echo "$w"; fi
done >"$LOG/warnings.txt"
N_WARN=$(wc -l <"$LOG/warnings.txt" | tr -d ' ')

# ---------------------------------------------------------------- ANSWERS.md
# Every "Answer:" line starts an answer section that runs until the next
# heading. A section counts as answered when it has any text in it.
N_QUESTIONS=0; N_ANSWERED=0
if [ -f ANSWERS.md ]; then
    read -r N_QUESTIONS N_ANSWERED < <(awk '
        function close_section() { if (open) { q++; if (has) a++ } open = 0; has = 0 }
        { line = $0; gsub(/<!--.*-->/, "", line) }
        line ~ /^[ \t]*(\*\*)?Answer:?(\*\*)?[ \t]*$/ { close_section(); open = 1; next }
        line ~ /^[ \t]*#/ { close_section(); next }
        open && line ~ /[^ \t\r]/ { has = 1 }
        END { close_section(); printf "%d %d\n", q, a }
    ' ANSWERS.md)
fi
# A missing ANSWERS.md, or one whose Answer: sections were removed, counts as empty.
ANSWERS_EMPTY=0
if is_student_file ANSWERS.md && [ "$N_ANSWERED" -eq 0 ]; then
    ANSWERS_EMPTY=1
fi

# ---------------------------------------------------------------- not compiled
TESTS_TOTAL=0; TESTS_PASSED=0; N_SAN=0; N_FORBIDDEN=0; N_CPPCHECK=0
CPPCHECK_RAN=false; TIMED_OUT=false; CRASHED=false
: >"$LOG/forbidden.txt"; : >"$LOG/cppcheck.txt"; : >"$LOG/sanitizers.txt"; : >"$LOG/results.txt"

if $COMPILED; then
    # ------------------------------------------------------------ forbidden symbols
    if [ -n "$FORBIDDEN" ]; then
        for c in ${STUDENT_C[@]+"${STUDENT_C[@]}"}; do
            obj="build/${c%.c}.o"
            [ -f "$obj" ] || continue
            nm -u "$obj" 2>/dev/null | awk '{print $NF}' | sed 's/^_//'
        done | sort -u >"$LOG/undefined.txt"
        for sym in $FORBIDDEN; do
            grep -qx "$sym" "$LOG/undefined.txt" && echo "$sym"
        done >"$LOG/forbidden.txt"
    fi
    N_FORBIDDEN=$(wc -l <"$LOG/forbidden.txt" | tr -d ' ')

    # ------------------------------------------------------------ cppcheck
    if command -v cppcheck >/dev/null 2>&1 && [ ${#STUDENT_C[@]} -gt 0 ]; then
        CPPCHECK_RAN=true
        # shellcheck disable=SC2086
        cppcheck --enable=warning,style,performance,portability --std=c11 --language=c \
            --quiet --error-exitcode=0 \
            --suppress=missingIncludeSystem --suppress=missingInclude \
            --suppress=unusedFunction --suppress=checkersReport \
            --suppress=constParameter --suppress=constParameterPointer \
            --suppress=constParameterCallback --suppress=unmatchedSuppression \
            --template='{file}:{line}:{column}: {severity}: {message} [{id}]' \
            $INCLUDES ${STUDENT_C[@]+"${STUDENT_C[@]}"} 2>"$LOG/cppcheck-raw.txt" >/dev/null
        grep -E ': (error|warning|style|performance|portability): ' "$LOG/cppcheck-raw.txt" | sort -u |
        while IFS= read -r finding; do
            if is_student_file "${finding%%:*}"; then echo "$finding"; fi
        done >"$LOG/cppcheck.txt"
        N_CPPCHECK=$(wc -l <"$LOG/cppcheck.txt" | tr -d ' ')
    fi

    # ------------------------------------------------------------ tests
    BIN=./build/test_runner
    "$BIN" -l 2>/dev/null | sed -n 's/^TEST:\([A-Za-z0-9_]*\)$/\1/p' >"$LOG/test-list.txt"
    TESTS_TOTAL=$(wc -l <"$LOG/test-list.txt" | tr -d ' ')

    # Pull "file:line:test_name:PASS|FAIL[:msg]" lines out of Unity output.
    parse_results() {
        awk '{
            if (match($0, /[A-Za-z0-9_.\/-]+:[0-9]+:test_[A-Za-z0-9_]+:(PASS|FAIL|IGNORE)/)) {
                rest = substr($0, RSTART)
                n = split(rest, p, ":")
                msg = ""
                for (i = 5; i <= n; i++) msg = msg (i > 5 ? ":" : "") p[i]
                sub(/^ +/, "", msg)
                print p[3] "\t" p[4] "\t" p[1] ":" p[2] "\t" msg
            }
        }' "$@"
    }

    ${TIMEOUT[@]+"${TIMEOUT[@]}"} "$BIN" >"$LOG/run-all.out" 2>"$LOG/san-all.log"
    RUN_RC=$?
    parse_results "$LOG/run-all.out" >"$LOG/results.txt"

    # Tests with no result line either hung or crashed the binary.
    missing() {
        while IFS= read -r t; do
            grep -q "^$t"$'\t' "$LOG/results.txt" || echo "$t"
        done <"$LOG/test-list.txt"
    }
    if [ "$(missing | wc -l | tr -d ' ')" -gt 0 ]; then
        first=1; timeouts=0
        for t in $(missing); do
            if [ "$RUN_RC" -eq 124 ] && [ $first -eq 1 ]; then
                # The full run was killed while this test was running.
                printf '%s\tFAIL\t-\tinfinite loop or too slow (stopped after 5 s)\n' "$t" >>"$LOG/results.txt"
                TIMED_OUT=true; timeouts=1; first=0
                continue
            fi
            first=0
            if [ $timeouts -ge 2 ]; then
                printf '%s\tFAIL\t-\tnot run: earlier tests hit the 5 s limit\n' "$t" >>"$LOG/results.txt"
                continue
            fi
            ${TIMEOUT[@]+"${TIMEOUT[@]}"} "$BIN" -n "$t" >"$LOG/run-one.out" 2>>"$LOG/san-one.log"
            rc=$?
            parse_results "$LOG/run-one.out" | grep "^$t"$'\t' >>"$LOG/results.txt"
            if ! grep -q "^$t"$'\t' "$LOG/results.txt"; then
                if [ $rc -eq 124 ]; then
                    printf '%s\tFAIL\t-\tinfinite loop or too slow (stopped after 5 s)\n' "$t" >>"$LOG/results.txt"
                    TIMED_OUT=true; timeouts=$((timeouts + 1))
                else
                    printf '%s\tFAIL\t-\tcrashed (exit code %d); run make test to see why\n' "$t" "$rc" >>"$LOG/results.txt"
                    CRASHED=true
                fi
            fi
        done
    fi
    TESTS_PASSED=$(awk -F'\t' '$2 == "PASS"' "$LOG/results.txt" | wc -l | tr -d ' ')

    # ------------------------------------------------------------ sanitizers
    # One line per distinct report: "<kind>\t<location>".
    cat "$LOG"/san-*.log 2>/dev/null | awk '
        function loc_of(s,   r) {
            if (s ~ /libsanitizer|compiler-rt/) return ""
            if (!match(s, /(src|tests)\/[A-Za-z0-9_.\/-]+:[0-9]+/)) return ""
            r = substr(s, RSTART, RLENGTH)
            # keep only the part from the last src/ or tests/ folder on
            while (match(r, /\/(src|tests)\//)) r = substr(r, RSTART + 1)
            return r
        }
        function flush() {
            if (pending) { print kind "\t" (best != "" ? best : (anyloc != "" ? anyloc : (first != "" ? first : "unknown"))) }
            pending = 0; best = ""; anyloc = ""; first = ""; frames = 0
        }
        /==[0-9]+==ERROR: AddressSanitizer: / {
            flush()
            k = $0; sub(/.*AddressSanitizer: /, "", k); split(k, w, " ")
            kind = w[1]; pending = 1; next
        }
        /==[0-9]+==ERROR: LeakSanitizer: / { flush(); next }
        /^(Direct|Indirect) leak of / { flush(); kind = "memory-leak"; pending = 1; next }
        / runtime error: / {
            flush()
            l = loc_of($0); m = $0; sub(/.* runtime error: /, "", m)
            if (m ~ /integer overflow/)                 kind = "integer-overflow"
            else if (m ~ /out of bounds/)               kind = "index-out-of-bounds"
            else if (m ~ /null pointer/)                kind = "null-pointer"
            else if (m ~ /insufficient space/)          kind = "out-of-bounds-access"
            else if (m ~ /shift/)                       kind = "invalid-shift"
            else if (m ~ /misaligned/)                  kind = "misaligned-access"
            else if (m ~ /divi[ds]/)                    kind = "division-by-zero"
            else if (m ~ /not a valid value/)           kind = "invalid-value"
            else                                        kind = "undefined-behavior"
            print kind "\t" (l != "" ? l : "unknown"); next
        }
        pending && /^ *#[0-9]+ 0x[0-9a-f]+ / {
            frames++
            l = loc_of($0)
            if (first == "") { f = $0; sub(/.* in /, "", f); first = f }
            if (best == "" && l ~ /^src\//) best = l
            if (anyloc == "" && l != "") anyloc = l
            next
        }
        pending && /^SUMMARY:|^Shadow bytes|^$/ { if (frames > 0) flush() }
        END { flush() }
    ' | sort -u | awk -F'\t' '
        # One bug often triggers several reports at the same line (e.g. ASan
        # "stack-buffer-overflow" plus UBSan "index out of bounds"): count it once.
        {
            key = ($2 == "unknown" ? $1 : $2)
            if (!(key in kinds)) { order[++n] = key; loc[key] = $2; kinds[key] = $1 }
            else if (index(", " kinds[key] ", ", ", " $1 ", ") == 0) kinds[key] = kinds[key] ", " $1
        }
        END { for (i = 1; i <= n; i++) print kinds[order[i]] "\t" loc[order[i]] }
    ' >"$LOG/sanitizers.txt"
    N_SAN=$(wc -l <"$LOG/sanitizers.txt" | tr -d ' ')
fi

# ---------------------------------------------------------------- arithmetic
if $COMPILED && [ "$TESTS_TOTAL" -gt 0 ]; then
    TEST_SCORE=$(( (TEST_POINTS * TESTS_PASSED * 2 + TESTS_TOTAL) / (TESTS_TOTAL * 2) ))
else
    TEST_SCORE=0
fi
D_WARN=$((N_WARN * PTS_WARNING))
D_CPP=0; [ "$CPPCHECK_COUNTS" = 1 ] && D_CPP=$((N_CPPCHECK * PTS_CPPCHECK))
D_SAN=$((N_SAN * PTS_SANITIZER))
D_FORB=$((N_FORBIDDEN * PTS_FORBIDDEN))
D_ANS=$((ANSWERS_EMPTY * PTS_ANSWERS))
QUALITY=$((QUALITY_POINTS - D_WARN - D_CPP - D_SAN - D_FORB - D_ANS))
[ $QUALITY -lt 0 ] && QUALITY=0
if $COMPILED; then TOTAL=$((TEST_SCORE + QUALITY)); else TOTAL=0; QUALITY=0; fi
PASSED=false; [ $TOTAL -ge $PASS_THRESHOLD ] && PASSED=true
CPP_ACTIVE=false; [ "$CPPCHECK_COUNTS" = 1 ] && CPP_ACTIVE=true

# ---------------------------------------------------------------- score.json
FAILED_JSON=$(awk -F'\t' '$2 != "PASS" { printf "%s\"%s\"", (n++ ? "," : ""), $1 }' "$LOG/results.txt")
cat >"$OUT/score.json" <<EOF
{
  "version": 1,
  "task": "$TASK",
  "stage": $TASK_STAGE,
  "compiled": $COMPILED,
  "total": $TOTAL,
  "pass_threshold": $PASS_THRESHOLD,
  "passed": $PASSED,
  "tests": {
    "points": $TEST_SCORE,
    "max": $TEST_POINTS,
    "passed": $TESTS_PASSED,
    "total": $TESTS_TOTAL,
    "timed_out": $TIMED_OUT,
    "crashed": $CRASHED,
    "failed": [$FAILED_JSON]
  },
  "quality": {
    "points": $QUALITY,
    "max": $QUALITY_POINTS,
    "deductions": {
      "compiler_warnings": {"count": $N_WARN, "each": $PTS_WARNING, "points": $D_WARN},
      "cppcheck": {"count": $N_CPPCHECK, "each": $PTS_CPPCHECK, "points": $D_CPP, "active": $CPP_ACTIVE, "ran": $CPPCHECK_RAN},
      "sanitizer_reports": {"count": $N_SAN, "each": $PTS_SANITIZER, "points": $D_SAN},
      "forbidden_symbols": {"count": $N_FORBIDDEN, "each": $PTS_FORBIDDEN, "points": $D_FORB},
      "answers_empty": {"count": $ANSWERS_EMPTY, "each": $PTS_ANSWERS, "points": $D_ANS}
    }
  },
  "answers": {"questions": $N_QUESTIONS, "answered": $N_ANSWERED}
}
EOF

# ---------------------------------------------------------------- score.md
{
    if $PASSED; then mark="✅ passed"; else mark="❌ below $PASS_THRESHOLD"; fi
    echo "## Score: $TOTAL / 100 — $mark"
    echo
    echo "Task \`$TASK\` (stage $TASK_STAGE). Pass threshold: $PASS_THRESHOLD."
    echo
    if ! $COMPILED; then
        echo "### Does not compile → score 0"
        echo
        echo "Fix the first error below, then build again (\`make\`)."
        echo
        echo '```text'
        grep -E 'error|undefined reference|ld returned' "$LOG/build.txt" | head -n 25
        echo '```'
    else
        echo "| Part | Points |"
        echo "|---|---|"
        echo "| Tests ($TESTS_PASSED of $TESTS_TOTAL passed) | $TEST_SCORE / $TEST_POINTS |"
        echo "| Code quality | $QUALITY / $QUALITY_POINTS |"
        echo "| **Total** | **$TOTAL / 100** |"
        echo
        echo "### Code quality deductions"
        echo
        echo "| Check | Count | Points |"
        echo "|---|---|---|"
        echo "| Compiler warnings (−$PTS_WARNING each) | $N_WARN | $(neg $D_WARN) |"
        if ! $CPPCHECK_RAN; then
            echo "| cppcheck (−$PTS_CPPCHECK each from stage 5) | not run: cppcheck is not installed | 0 |"
        elif $CPP_ACTIVE; then
            echo "| cppcheck findings (−$PTS_CPPCHECK each) | $N_CPPCHECK | $(neg $D_CPP) |"
        else
            echo "| cppcheck findings (notes only before stage 5) | $N_CPPCHECK | 0 |"
        fi
        echo "| Distinct sanitizer reports (−$PTS_SANITIZER each) | $N_SAN | $(neg $D_SAN) |"
        echo "| Forbidden functions used (−$PTS_FORBIDDEN each) | $N_FORBIDDEN | $(neg $D_FORB) |"
        echo "| \`ANSWERS.md\` left empty (−$PTS_ANSWERS) | $([ $ANSWERS_EMPTY = 1 ] && echo yes || echo "no ($N_ANSWERED of $N_QUESTIONS answered)") | $(neg $D_ANS) |"
        echo
        if [ "$TESTS_PASSED" -lt "$TESTS_TOTAL" ]; then
            echo "### Failing tests"
            echo
            awk -F'\t' '$2 != "PASS" {
                where = ($3 == "-" ? "" : " (" $3 ")")
                printf "- `%s`%s: %s\n", $1, where, ($4 == "" ? "failed" : $4)
            }' "$LOG/results.txt"
            echo
        fi
        if [ "$N_WARN" -gt 0 ]; then
            echo "### Compiler warnings"
            echo
            echo '```text'; cat "$LOG/warnings.txt"; echo '```'
            echo
        fi
        if [ "$N_SAN" -gt 0 ]; then
            echo "### Sanitizer reports"
            echo
            echo "Run \`make test\` to see the full reports with stack traces."
            echo
            awk -F'\t' '{ printf "- **%s** at `%s`\n", $1, $2 }' "$LOG/sanitizers.txt"
            echo
        fi
        if [ "$N_FORBIDDEN" -gt 0 ]; then
            echo "### Forbidden functions"
            echo
            echo "From stage 3 on, dynamic memory is not allowed. Found in your code:"
            echo
            sed 's/^/- `/; s/$/`/' "$LOG/forbidden.txt"
            echo
        fi
        if [ "$N_CPPCHECK" -gt 0 ]; then
            if $CPP_ACTIVE; then echo "### cppcheck findings"; else echo "### cppcheck notes (no points deducted before stage 5)"; fi
            echo
            echo '```text'; cat "$LOG/cppcheck.txt"; echo '```'
            echo
        fi
    fi
} >"$OUT/score.md"

cat "$OUT/score.md"
exit 0
