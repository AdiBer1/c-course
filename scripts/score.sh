#!/usr/bin/env bash
# score.sh - build, test and score one task, exactly like CI does.
#
#   scripts/score.sh tasks/07-command-parser      (or: make score, inside the task)
#   scripts/score.sh --phase build <task-dir>     one phase only (used by CI)
#
# Phases (all four, in this order, when --phase is not given):
#   build    clean, compile, collect compiler warnings and forbidden symbols
#   analyze  run cppcheck on the student's .c files
#   test     run the tests (needs build), collect sanitizer reports
#   report   compute the score from the logs the other phases left behind
#
# Every phase reads and writes <task>/build/score-logs/. CI runs each phase in
# its own job and passes that folder between jobs, so CI and `make score`
# always compute the score with this same code.
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
#       sanitizer report        -10 each distinct report (one per code location)
#       forbidden symbol        -15 each symbol used
#       ANSWERS.md left empty   -10
#
# Exit code: 0 when the phase ran (even if the code is bad), 2 on internal error.

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
count_lines() { if [ -f "$1" ]; then wc -l <"$1" | tr -d ' '; else echo 0; fi; }
flag() { if [ -f "$1" ] && [ "$(cat "$1")" = true ]; then echo true; else echo false; fi; }

PHASE=all
if [ "${1:-}" = "--phase" ]; then
    PHASE=${2:-}
    [ $# -ge 2 ] && shift 2
fi
case $PHASE in all | build | analyze | test | report) ;; *) die "unknown phase: $PHASE" ;; esac
[ $# -ge 1 ] || die "usage: score.sh [--phase build|analyze|test|report] <task-dir>"
TASK_DIR=$(cd "$1" 2>/dev/null && pwd) || die "no such directory: $1"
[ -f "$TASK_DIR/task.mk" ] || die "$TASK_DIR is not a task (no task.mk)"
cd "$TASK_DIR" || die "cannot enter $TASK_DIR"

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

OUT=build
LOG=build/score-logs

is_student_file() {
    local f
    for f in $STUDENT_FILES; do [ "$f" = "$1" ] && return 0; done
    return 1
}

# ================================================================ phase: build
phase_build() {
    make -s --no-print-directory clean >/dev/null 2>&1
    mkdir -p "$LOG"

    make --no-print-directory all >"$LOG/build.txt" 2>&1
    if [ $? -eq 0 ]; then echo true; else echo false; fi >"$LOG/compiled"

    # Compiler warnings in the student's own files (distinct lines).
    grep -E '^[^ :]+:[0-9]+:[0-9]+: warning: ' "$LOG/build.txt" 2>/dev/null | sort -u |
    while IFS= read -r w; do
        if is_student_file "${w%%:*}"; then echo "$w"; fi
    done >"$LOG/warnings.txt"

    # Forbidden functions: undefined symbols in the student's object files.
    : >"$LOG/forbidden.txt"
    if [ "$(flag "$LOG/compiled")" = true ] && [ -n "$FORBIDDEN" ]; then
        for c in ${STUDENT_C[@]+"${STUDENT_C[@]}"}; do
            obj="build/${c%.c}.o"
            [ -f "$obj" ] || continue
            nm -u "$obj" 2>/dev/null | awk '{print $NF}' | sed 's/^_//'
        done | sort -u >"$LOG/undefined.txt"
        for sym in $FORBIDDEN; do
            grep -qx "$sym" "$LOG/undefined.txt" && echo "$sym"
        done >"$LOG/forbidden.txt"
    fi
}

# ================================================================ phase: analyze
phase_analyze() {
    mkdir -p "$LOG"
    : >"$LOG/cppcheck.txt"
    if ! command -v cppcheck >/dev/null 2>&1 || [ ${#STUDENT_C[@]} -eq 0 ]; then
        echo false >"$LOG/cppcheck-ran"
        return
    fi
    # shellcheck disable=SC2086  # INCLUDES is a list of -I flags
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
    echo true >"$LOG/cppcheck-ran"
}

# ================================================================ phase: test
phase_test() {
    mkdir -p "$LOG"
    if [ "$(flag "$LOG/compiled")" != true ]; then
        echo "score.sh: not compiled, tests skipped" >&2
        return
    fi

    local TIMEOUT=()
    if command -v timeout >/dev/null 2>&1; then TIMEOUT=(timeout 5s); fi
    local leaks=0
    [ "$(uname -s)" = Linux ] && leaks=1
    export ASAN_OPTIONS="halt_on_error=0:abort_on_error=0:detect_leaks=$leaks:detect_stack_use_after_return=1:print_summary=1"
    export UBSAN_OPTIONS="halt_on_error=0:print_stacktrace=1"

    rm -f "$LOG"/san-*.log "$LOG/results.txt"
    local timed_out=false crashed=false
    local BIN=./build/test_runner
    chmod +x "$BIN" 2>/dev/null # CI artifacts do not keep the executable bit
    "$BIN" -l 2>/dev/null | sed -n 's/^TEST:\([A-Za-z0-9_]*\)$/\1/p' >"$LOG/test-list.txt"

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
    local run_rc=$?
    parse_results "$LOG/run-all.out" >"$LOG/results.txt"

    # Tests with no result line either hung or crashed the binary.
    missing() {
        while IFS= read -r t; do
            grep -q "^$t"$'\t' "$LOG/results.txt" || echo "$t"
        done <"$LOG/test-list.txt"
    }
    if [ "$(missing | wc -l | tr -d ' ')" -gt 0 ]; then
        local first=1 timeouts=0 t rc
        for t in $(missing); do
            if [ "$run_rc" -eq 124 ] && [ $first -eq 1 ]; then
                # The full run was killed while this test was running.
                printf '%s\tFAIL\t-\tinfinite loop or too slow (stopped after 5 s)\n' "$t" >>"$LOG/results.txt"
                timed_out=true; timeouts=1; first=0
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
                    timed_out=true; timeouts=$((timeouts + 1))
                else
                    printf '%s\tFAIL\t-\tcrashed (exit code %d); run make test to see why\n' "$t" "$rc" >>"$LOG/results.txt"
                    crashed=true
                fi
            fi
        done
    fi
    echo "$timed_out" >"$LOG/timed_out"
    echo "$crashed" >"$LOG/crashed"

    # ------------------------------------------------------------ sanitizers
    # sanitizers-raw.txt: one line per report, "<tool>\t<kind>\t<location>",
    # tool = ASan (AddressSanitizer, including leaks) or UBSan.
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
            if (pending) { print "ASan\t" kind "\t" (best != "" ? best : (anyloc != "" ? anyloc : (first != "" ? first : "unknown"))) }
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
            print "UBSan\t" kind "\t" (l != "" ? l : "unknown"); next
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
    ' | sort -u >"$LOG/sanitizers-raw.txt"

    # sanitizers.txt (used for the score): one bug often triggers several
    # reports at the same line (e.g. ASan "stack-buffer-overflow" plus UBSan
    # "index out of bounds"), so count each code location once.
    awk -F'\t' '
        {
            key = ($3 == "unknown" ? $2 : $3)
            if (!(key in kinds)) { order[++n] = key; loc[key] = $3; kinds[key] = $2 }
            else if (index(", " kinds[key] ", ", ", " $2 ", ") == 0) kinds[key] = kinds[key] ", " $2
        }
        END { for (i = 1; i <= n; i++) print kinds[order[i]] "\t" loc[order[i]] }
    ' "$LOG/sanitizers-raw.txt" >"$LOG/sanitizers.txt"

    echo true >"$LOG/tests-ran"
}

# ================================================================ phase: report
phase_report() {
    mkdir -p "$LOG"
    local COMPILED TESTS_RAN TIMED_OUT CRASHED CPPCHECK_RAN
    COMPILED=$(flag "$LOG/compiled")
    TESTS_RAN=$(flag "$LOG/tests-ran")
    TIMED_OUT=$(flag "$LOG/timed_out")
    CRASHED=$(flag "$LOG/crashed")
    CPPCHECK_RAN=$(flag "$LOG/cppcheck-ran")
    [ -f "$LOG/results.txt" ] || : >"$LOG/results.txt"
    [ -f "$LOG/build.txt" ] || : >"$LOG/build.txt"

    local N_WARN N_FORBIDDEN N_CPPCHECK N_SAN TESTS_TOTAL TESTS_PASSED
    N_WARN=$(count_lines "$LOG/warnings.txt")
    N_FORBIDDEN=0; N_CPPCHECK=0; N_SAN=0; TESTS_TOTAL=0; TESTS_PASSED=0
    if $COMPILED; then
        N_FORBIDDEN=$(count_lines "$LOG/forbidden.txt")
        N_CPPCHECK=$(count_lines "$LOG/cppcheck.txt")
        if $TESTS_RAN; then
            TESTS_TOTAL=$(count_lines "$LOG/test-list.txt")
            TESTS_PASSED=$(awk -F'\t' '$2 == "PASS"' "$LOG/results.txt" | wc -l | tr -d ' ')
            N_SAN=$(count_lines "$LOG/sanitizers.txt")
        fi
    fi

    # ANSWERS.md: every "Answer:" line starts an answer section that runs until
    # the next heading. A section counts as answered when it has any text in it.
    local N_QUESTIONS=0 N_ANSWERED=0
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
    local ANSWERS_EMPTY=0
    if is_student_file ANSWERS.md && [ "$N_ANSWERED" -eq 0 ]; then
        ANSWERS_EMPTY=1
    fi

    # ------------------------------------------------------------ arithmetic
    local TEST_SCORE=0
    if $COMPILED && [ "$TESTS_TOTAL" -gt 0 ]; then
        TEST_SCORE=$(( (TEST_POINTS * TESTS_PASSED * 2 + TESTS_TOTAL) / (TESTS_TOTAL * 2) ))
    fi
    local D_WARN D_CPP=0 D_SAN D_FORB D_ANS QUALITY TOTAL PASSED=false CPP_ACTIVE=false
    D_WARN=$((N_WARN * PTS_WARNING))
    [ "$CPPCHECK_COUNTS" = 1 ] && D_CPP=$((N_CPPCHECK * PTS_CPPCHECK))
    D_SAN=$((N_SAN * PTS_SANITIZER))
    D_FORB=$((N_FORBIDDEN * PTS_FORBIDDEN))
    D_ANS=$((ANSWERS_EMPTY * PTS_ANSWERS))
    QUALITY=$((QUALITY_POINTS - D_WARN - D_CPP - D_SAN - D_FORB - D_ANS))
    [ $QUALITY -lt 0 ] && QUALITY=0
    if $COMPILED; then TOTAL=$((TEST_SCORE + QUALITY)); else TOTAL=0; QUALITY=0; fi
    [ $TOTAL -ge $PASS_THRESHOLD ] && PASSED=true
    [ "$CPPCHECK_COUNTS" = 1 ] && CPP_ACTIVE=true

    # ------------------------------------------------------------ score.json
    local FAILED_JSON
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

    # ------------------------------------------------------------ score.md
    {
        local mark
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
            if ! $TESTS_RAN; then
                echo "> ⚠️ The tests did not run, so the test points are 0. Run \`make test\` to see why."
                echo
            fi
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
            echo "| Compiler warnings (−$PTS_WARNING each) | $N_WARN | $(neg "$D_WARN") |"
            if ! $CPPCHECK_RAN; then
                echo "| cppcheck (−$PTS_CPPCHECK each from stage 5) | not run: cppcheck is not installed | 0 |"
            elif $CPP_ACTIVE; then
                echo "| cppcheck findings (−$PTS_CPPCHECK each) | $N_CPPCHECK | $(neg "$D_CPP") |"
            else
                echo "| cppcheck findings (notes only before stage 5) | $N_CPPCHECK | 0 |"
            fi
            echo "| Distinct sanitizer reports (−$PTS_SANITIZER each) | $N_SAN | $(neg "$D_SAN") |"
            echo "| Forbidden functions used (−$PTS_FORBIDDEN each) | $N_FORBIDDEN | $(neg "$D_FORB") |"
            echo "| \`ANSWERS.md\` left empty (−$PTS_ANSWERS) | $([ $ANSWERS_EMPTY = 1 ] && echo yes || echo "no ($N_ANSWERED of $N_QUESTIONS answered)") | $(neg "$D_ANS") |"
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
                echo "This task does not allow these functions (see its README). Found in your code:"
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
}

case $PHASE in
    build) phase_build ;;
    analyze) phase_analyze ;;
    test) phase_test ;;
    report) phase_report ;;
    all)
        phase_build
        phase_analyze
        phase_test
        phase_report
        ;;
esac
exit 0
