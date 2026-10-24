#!/usr/bin/env bash
# ci-report.sh - explain one part of the result as a CI check.
#
#   scripts/ci-report.sh <check> <task-dir>
#   check: build | tests | asan | ubsan | static | grading
#
# Reads only what scripts/score.sh left in <task-dir>/build/score-logs/ (and
# build/score.json for "grading"). It never computes points itself: the score
# always comes from score.sh, so a check can never change the grade.
#
# Writes a Markdown report to $GITHUB_STEP_SUMMARY (stdout when run locally)
# and, inside GitHub Actions, file/line annotations to stdout.
# Exit code: 0 = check passed, 1 = check failed, 2 = usage or missing results.

set -uo pipefail

CHECK=${1:?usage: ci-report.sh <build|tests|asan|ubsan|static|grading> <task-dir>}
TASK=${2:?usage: ci-report.sh <check> <task-dir>}
TASK=${TASK%/}
LOG="$TASK/build/score-logs"
SUMMARY=${GITHUB_STEP_SUMMARY:-/dev/stdout}
IN_CI=${GITHUB_ACTIONS:-false}

# ---------------------------------------------------------------- helpers
say() { printf '%s\n' "$@" >>"$SUMMARY"; }

# Text from student code or logs must not break out of a Markdown code block
# or table cell.
fence() { sed 's/```/'"'''"'/g' "$@"; }
cell() { printf '%s' "$1" | tr -d '\r\n' | sed 's/|/\\|/g; s/`/'"'"'/g' | cut -c1-200; }

# GitHub workflow-command escaping: student text can never start a new command.
esc_data() { local s=$1; s=${s//'%'/'%25'}; s=${s//$'\r'/'%0D'}; s=${s//$'\n'/'%0A'}; printf '%s' "$s"; }
esc_prop() { local s; s=$(esc_data "$1"); s=${s//':'/'%3A'}; s=${s//','/'%2C'}; printf '%s' "$s"; }
# annotate <error|warning|notice> <file-or-empty> <line-or-empty> <title> <message>
annotate() {
    [ "$IN_CI" = true ] || return 0
    local props
    props="title=$(esc_prop "$4")"
    [ -n "$2" ] && props="file=$(esc_prop "$2"),$props"
    [ -n "$3" ] && props="line=$3,$props"
    printf '::%s %s::%s\n' "$1" "$props" "$(esc_data "${5:0:500}")"
}

flag() { [ -f "$LOG/$1" ] && [ "$(cat "$LOG/$1")" = true ]; }
lines() { if [ -f "$1" ]; then grep -c . "$1"; else echo 0; fi; }

config() { make -s --no-print-directory -C "$TASK" config 2>/dev/null | sed -n "s/^$1=//p"; }

require() {
    for f in "$@"; do
        if [ ! -f "$LOG/$f" ]; then
            say "### ⚠️ No results" "" "This check found no results (\`$f\` is missing). That is a CI problem, not your code: re-run the workflow."
            exit 2
        fi
    done
}

# "src/x.c:12" -> annotate at tasks/NN/src/x.c line 12
loc_file() { printf '%s' "${1%%:*}"; }
loc_line() { local l=${1#*:}; l=${l%%:*}; [[ $l =~ ^[0-9]+$ ]] && printf '%s' "$l"; }

# ---------------------------------------------------------------- build
report_build() {
    require compiled build.txt
    local flags gcc_version
    flags=$(config STUDENT_CFLAGS)
    gcc_version=$(gcc --version 2>/dev/null | head -n1)

    if ! flag compiled; then
        say "### 🔨 Build: FAILED ❌" ""
        say "Your code does not compile, so the automated score is **0** and the tests did not run." ""
        say "**First errors:**" "" '```text'
        grep -E ': (fatal )?error: |undefined reference|ld returned' "$LOG/build.txt" | head -n 20 | fence >>"$SUMMARY"
        say '```' ""
        grep -E '^[^ :]+:[0-9]+:[0-9]+: (fatal )?error: ' "$LOG/build.txt" | head -n 10 |
        while IFS= read -r e; do
            annotate error "$TASK/$(loc_file "$e")" "$(loc_line "$e")" "Compile error" "${e#*: error: }"
        done
        say "**Reproduce locally:** \`cd $TASK && make\`" ""
        say "**What to do:** fix the **first** error; later errors are often caused by it." ""
        return 1
    fi

    local n_warn n_forb
    n_warn=$(lines "$LOG/warnings.txt")
    n_forb=$(lines "$LOG/forbidden.txt")
    local extra=()
    [ "$n_warn" -gt 0 ] && extra+=("$n_warn warning$([ "$n_warn" = 1 ] || echo s)")
    [ "$n_forb" -gt 0 ] && extra+=("$n_forb forbidden function$([ "$n_forb" = 1 ] || echo s)")
    if [ ${#extra[@]} -gt 0 ]; then
        say "### 🔨 Build: COMPILED ✅, but with $(IFS=,; echo "${extra[*]}" | sed 's/,/ and /') ⚠️" ""
        say "The code compiles, so this check is green; the problems below cost points in **Grading / Automated Score**." ""
    else
        say "### 🔨 Build: PASSED ✅" ""
    fi
    say "Compiler: \`$gcc_version\`" "" "Flags for your code: \`$flags\`" ""
    say "| Check | Result | Affects your grade |" "|---|---|---|"
    say "| Compiles | ✅ yes | does not compile = score 0 |"
    say "| Compiler warnings | $([ "$n_warn" = 0 ] && echo "✅ none" || echo "⚠️ $n_warn") | −5 each |"
    say "| Forbidden functions | $([ "$n_forb" = 0 ] && echo "✅ none" || echo "❌ $(tr '\n' ' ' <"$LOG/forbidden.txt")") | −15 each |" ""
    if [ "$n_warn" -gt 0 ]; then
        say "**Warnings in your files:**" "" '```text'
        fence "$LOG/warnings.txt" >>"$SUMMARY"
        say '```' "" "Warnings do not stop the build, but each one costs points and often points at a real bug. Reproduce: \`cd $TASK && make clean && make\`." ""
        while IFS= read -r w; do
            annotate warning "$TASK/$(loc_file "$w")" "$(loc_line "$w")" "Compiler warning (−5)" "${w#*: warning: }"
        done <"$LOG/warnings.txt"
    fi
    if [ "$n_forb" -gt 0 ]; then
        say "**Forbidden functions** found in your object files: $(sed 's/.*/`&`/' "$LOG/forbidden.txt" | tr '\n' ' '). The task README lists what is not allowed." ""
        while IFS= read -r f; do
            annotate error "" "" "Forbidden function (−15)" "$f is not allowed in this task"
        done <"$LOG/forbidden.txt"
    fi
    return 0
}

# ---------------------------------------------------------------- tests
report_tests() {
    if ! flag tests-ran; then
        say "### 🧪 Unit tests: DID NOT RUN ⚠️" "" "The tests did not run although the code compiled. Run \`cd $TASK && make test\` to see why; if it works locally, re-run the workflow."
        return 1
    fi
    local total passed failed points
    total=$(lines "$LOG/test-list.txt")
    passed=$(awk -F'\t' '$2 == "PASS"' "$LOG/results.txt" | wc -l | tr -d ' ')
    failed=$((total - passed))
    points=0
    [ "$total" -gt 0 ] && points=$(( (70 * passed * 2 + total) / (total * 2) ))

    if [ "$failed" -eq 0 ] && [ "$total" -gt 0 ]; then
        say "### 🧪 Unit tests: PASSED ✅" "" "**$passed/$total** tests passed → **$points/70** test points." ""
        return 0
    fi
    say "### 🧪 Unit tests: FAILED ❌" "" "**$passed/$total** tests passed → **$points/70** test points (proportional to the share that pass)." ""
    flag timed_out && say "⏱️ At least one test hit the **5-second limit**: an infinite loop, or code that waits for something that never happens." ""
    flag crashed && say "💥 At least one test **crashed** (for example a segmentation fault). The other tests still ran." ""
    say "| Test | Where | Message |" "|---|---|---|"
    awk -F'\t' '$2 != "PASS"' "$LOG/results.txt" | head -n 40 |
    while IFS=$'\t' read -r name _ where msg; do
        say "| \`$(cell "$name")\` | $( [ "$where" = - ] && echo "-" || echo "\`$(cell "$where")\`") | $(cell "${msg:-failed}") |"
        if [ "$where" != - ]; then
            annotate error "$TASK/$(loc_file "$where")" "$(loc_line "$where")" "Test failed: $name" "${msg:-failed}"
        else
            annotate error "" "" "Test failed: $name" "${msg:-failed}"
        fi
    done
    say "" "**Reproduce locally:** \`cd $TASK && make test\`, or one test only: \`./build/test_runner -n <test_name>\` after \`make\`." ""
    say "**Read a failure like this:** \`Expected 9 Was 2\` means the test wanted 9 and your function returned 2. Open the test file at the line shown to see the exact call." ""
    return 1
}

# ---------------------------------------------------------------- sanitizers
describe() {
    case $1 in
        heap-buffer-overflow) echo "Heap buffer overflow: read or write past the end of a heap block" ;;
        stack-buffer-overflow) echo "Stack buffer overflow: read or write past the end of a local array" ;;
        global-buffer-overflow) echo "Global buffer overflow: read or write past the end of a global or static array" ;;
        stack-use-after-return) echo "Use after return: a pointer to a local variable used after its function returned" ;;
        stack-use-after-scope) echo "Use after scope: a pointer to a variable used after its block ended" ;;
        heap-use-after-free) echo "Use after free: memory used after free()" ;;
        attempting) echo "Invalid or double free()" ;;
        SEGV) echo "Invalid memory access (segmentation fault): usually a NULL, uninitialised or dangling pointer" ;;
        stack-overflow) echo "Stack overflow: usually endless recursion or a huge local array" ;;
        memory-leak) echo "Memory leak: memory allocated and never freed" ;;
        integer-overflow) echo "Signed integer overflow: the result does not fit in the type" ;;
        index-out-of-bounds) echo "Array index out of bounds" ;;
        null-pointer) echo "Use of a NULL pointer" ;;
        out-of-bounds-access) echo "Load or store outside an object" ;;
        invalid-shift) echo "Invalid shift: shift amount too large or negative, or a 1 shifted into the sign bit" ;;
        misaligned-access) echo "Misaligned memory access" ;;
        division-by-zero) echo "Division by zero" ;;
        invalid-value) echo "Invalid value loaded (for example a bool that is neither 0 nor 1)" ;;
        *) echo "$1" ;;
    esac
}

report_sanitizer() {   # $1 = ASan | UBSan
    local tool=$1 title emoji
    if [ "$tool" = ASan ]; then title="AddressSanitizer"; emoji="🛡️"; else title="UndefinedBehaviorSanitizer"; emoji="⚠️"; fi
    if ! flag tests-ran; then
        say "### $emoji $title: DID NOT RUN ⚠️" "" "The tests did not run, so nothing was checked."
        return 1
    fi
    local hits
    hits=$(awk -F'\t' -v t="$tool" '$1 == t' "$LOG/sanitizers-raw.txt" 2>/dev/null)
    if [ -z "$hits" ]; then
        say "### $emoji $title: PASSED ✅" ""
        if [ "$tool" = ASan ]; then
            say "No out-of-bounds access, use-after-free, use-after-return, invalid pointer or leak was detected while the tests ran."
        else
            say "No signed overflow, invalid shift, out-of-bounds index, NULL use or division by zero was detected while the tests ran."
        fi
        say "" "Only code that the tests execute is checked." ""
        return 0
    fi

    local n
    n=$(printf '%s\n' "$hits" | awk -F'\t' '{print $3}' | sort -u | grep -c .)
    say "### $emoji $title: FAILED ❌" "" "**Detected** at $n code location$([ "$n" = 1 ] || echo s):" ""
    say "| Problem | Location |" "|---|---|"
    printf '%s\n' "$hits" | sort -t$'\t' -k3,3 -k2,2 | while IFS=$'\t' read -r _ kind where; do
        say "| $(cell "$(describe "$kind")") | \`$(cell "$where")\` |"
        case $where in
            src/*) annotate error "$TASK/$(loc_file "$where")" "$(loc_line "$where")" "$title: $kind" "$(describe "$kind")" ;;
            *) annotate error "" "" "$title: $kind" "$(describe "$kind") at $where" ;;
        esac
    done
    say "" "**First report** (the full ones, with stack traces, appear in \`make test\`):" "" '```text'
    if [ "$tool" = ASan ]; then
        cat "$LOG"/san-*.log 2>/dev/null | awk '
            /==[0-9]+==ERROR: (AddressSanitizer|LeakSanitizer)/ { on = 1 }
            on { print; n++ }
            on && n > 3 && /^$/ { exit }
            n >= 30 { exit }' | fence >>"$SUMMARY"
    else
        cat "$LOG"/san-*.log 2>/dev/null | grep -A4 ' runtime error: ' | head -n 20 | fence >>"$SUMMARY"
    fi
    say '```' ""
    say "**How to read it:** the first line says what went wrong; the \`#0\`, \`#1\` ... lines are the call stack, newest call first. Look for the first frame in your \`src/\` file." ""
    say "**Reproduce locally:** \`cd $TASK && make test\` (sanitizers are always on in this course)." ""
    say "**Affects your grade:** −10 for each distinct code location. A line that both sanitizers report counts **once**." ""
    return 1
}

# ---------------------------------------------------------------- static analysis
report_static() {
    require cppcheck-ran
    if ! flag cppcheck-ran; then
        say "### 🔍 Static analysis: NOT RUN ⚠️" "" "cppcheck is not installed on the CI runner. That is a CI problem: re-run the workflow."
        return 1
    fi
    local n active version
    n=$(lines "$LOG/cppcheck.txt")
    active=$(config CPPCHECK_COUNTS)
    version=$(cppcheck --version 2>/dev/null)
    if [ "$n" -eq 0 ]; then
        say "### 🔍 Static analysis: PASSED ✅" "" "\`$version\` found nothing in your files." ""
        return 0
    fi
    local level=notice status="PASSED ✅ (notes only)" grade="no points before stage 5"
    if [ "$active" = 1 ]; then level=warning; status="FAILED ❌"; grade="−3 each (stage 5 and later)"; fi
    say "### 🔍 Static analysis: $status" "" "\`$version\` reported **$n** finding$([ "$n" = 1 ] || echo s) in your files. Affects your grade: **$grade**." ""
    say '```text'
    fence "$LOG/cppcheck.txt" >>"$SUMMARY"
    say '```' ""
    while IFS= read -r f; do
        annotate "$level" "$TASK/$(loc_file "$f")" "$(loc_line "$f")" "cppcheck" "${f#*: *: }"
    done <"$LOG/cppcheck.txt"
    say "cppcheck reads your code without running it. Each finding names the line and the reason in [brackets]." ""
    say "**Reproduce locally:** \`cd $TASK && make score\` (needs \`sudo apt install cppcheck\`)." ""
    [ "$active" = 1 ] && return 1
    return 0
}

# ---------------------------------------------------------------- grading
report_grading() {
    local json="$TASK/build/score.json"
    if [ ! -f "$json" ]; then
        say "### 🎓 Automated score: NO RESULT ⚠️" "" "score.json is missing. That is a CI problem: re-run the workflow."
        return 2
    fi
    local total
    total=$(sed -n 's/^  "total": \([0-9]*\),$/\1/p' "$json")
    if [ "${total:-0}" -ge 60 ]; then
        say "### 🎓 Automated score: $total/100 — PASSED ✅" "" "You reached the pass threshold of 60. **Next step:** schedule your oral defense." ""
    else
        say "### 🎓 Automated score: ${total:-0}/100 — BELOW 60 ❌" "" "**Next step:** fix what the other checks report, run \`make score\` locally, and push again." ""
    fi
    say "This is the same calculation as \`make score\` on your machine." "" "---" ""
    cat "$TASK/build/score.md" >>"$SUMMARY"
    [ "${total:-0}" -ge 60 ]
}

case $CHECK in
    build) report_build ;;
    tests) report_tests ;;
    asan) report_sanitizer ASan ;;
    ubsan) report_sanitizer UBSan ;;
    static) report_static ;;
    grading) report_grading ;;
    *) echo "ci-report.sh: unknown check $CHECK" >&2; exit 2 ;;
esac
