#!/usr/bin/env bash
# memcheck.sh - run the tests under Valgrind (informational, no points).
#
#   scripts/memcheck.sh <task-dir>        (or: make memcheck, inside the task)
#
# Builds a SEPARATE copy of the tests without sanitizers (Valgrind and
# AddressSanitizer cannot be combined) in build/memcheck/ and runs it under
# Valgrind memcheck. Its main value next to AddressSanitizer: it reports reads
# of UNINITIALISED memory, which gcc's sanitizers cannot detect.
# Never changes the score; always exits 0 unless called wrongly.

set -uo pipefail

TASK=${1:?usage: memcheck.sh <task-dir>}
TASK=${TASK%/}
SUMMARY=${GITHUB_STEP_SUMMARY:-/dev/stdout}
say() { printf '%s\n' "$@" >>"$SUMMARY"; }
cell() { printf '%s' "$1" | tr -d '\r\n' | sed 's/|/\\|/g; s/`/'"'"'/g' | cut -c1-200; }
esc_data() { local s=$1; s=${s//'%'/'%25'}; s=${s//$'\r'/'%0D'}; s=${s//$'\n'/'%0A'}; printf '%s' "$s"; }

if ! command -v valgrind >/dev/null 2>&1; then
    say "### 🧠 Memory analysis: NOT RUN" "" "Valgrind is not installed (\`sudo apt install valgrind\`)."
    exit 0
fi

LOGDIR="$TASK/build/memcheck"
mkdir -p "$LOGDIR"
if ! make -s --no-print-directory -C "$TASK" BUILD=build/memcheck SAN_FLAGS= all >"$LOGDIR/build.txt" 2>&1; then
    say "### 🧠 Memory analysis: skipped" "" "The code does not compile (see the Build check)."
    exit 0
fi

TIMEOUT=()
command -v timeout >/dev/null 2>&1 && TIMEOUT=(timeout 120s)
(cd "$TASK" && ${TIMEOUT[@]+"${TIMEOUT[@]}"} valgrind --error-exitcode=99 --leak-check=full \
    --show-leak-kinds=definite --errors-for-leak-kinds=definite --track-origins=yes \
    --num-callers=12 ./build/memcheck/test_runner) >"$LOGDIR/run.out" 2>"$LOGDIR/valgrind.txt"
rc=$?

# The student's .c files, by base name (Valgrind prints "parser.c:42").
mine=$(make -s --no-print-directory -C "$TASK" config 2>/dev/null | sed -n 's/^STUDENT_FILES=//p' |
       tr ' ' '\n' | grep '\.c$' | sed 's#.*/##' | tr '\n' ' ')

# One line per error: "<kind>\t<first location in a student file, or ?>"
awk -v mine=" $mine " '
    function flush() { if (kind != "") print kind "\t" (loc != "" ? loc : "?"); kind = ""; loc = "" }
    /^==[0-9]+== (Invalid (read|write|free)|Conditional jump or move depends on uninitialised|Use of uninitialised value|Syscall param .* uninitialised|Mismatched free|Source and destination overlap)/ {
        flush(); k = $0; sub(/^==[0-9]+== /, "", k); sub(/ of size [0-9]+/, "", k); kind = k; next
    }
    /^==[0-9]+== .* definitely lost in / { flush(); kind = "Memory leak (definitely lost)"; next }
    kind != "" && loc == "" && match($0, /\([A-Za-z0-9_.-]+\.c:[0-9]+\)$/) {
        l = substr($0, RSTART + 1, RLENGTH - 2); f = l; sub(/:.*/, "", f)
        if (index(mine, " " f " ")) loc = "src/" l
    }
    /^==[0-9]+== $/ { flush() }
    END { flush() }
' "$LOGDIR/valgrind.txt" | sort | uniq -c | sort -rn >"$LOGDIR/errors.txt"

if [ "$rc" -eq 124 ]; then
    say "### 🧠 Memory analysis: stopped after 120 s ⏱️ (informational)" "" "Under Valgrind code runs 20–50x slower; an infinite loop shows up in the Unit Tests check." ""
fi
if [ ! -s "$LOGDIR/errors.txt" ]; then
    say "### 🧠 Memory analysis: PASSED ✅" "" "Valgrind memcheck found no invalid access, no use of uninitialised memory and no definite leak while the tests ran." ""
else
    say "### 🧠 Memory analysis: issues found ℹ️ (informational)" ""
    say "**Does not affect your grade**, but these are real bugs that may hide on a microcontroller." ""
    say "| Count | Problem | Where |" "|---|---|---|"
    while read -r count rest; do
        kind=${rest%%$'\t'*}; where=${rest#*$'\t'}
        say "| $count | $(cell "$kind") | \`$(cell "$where")\` |"
        if [ "${GITHUB_ACTIONS:-false}" = true ]; then
            f=${where%%:*}; l=${where##*:}
            if [[ $f == src/* && $l =~ ^[0-9]+$ ]]; then
                echo "::notice file=$TASK/$f,line=$l,title=Valgrind (informational)::$(esc_data "$kind")"
            fi
        fi
    done <"$LOGDIR/errors.txt"
    say "" "**\"Conditional jump depends on uninitialised value\"** means an \`if\` or loop used a variable that was never given a value. AddressSanitizer cannot see this, Valgrind can." ""
    say "<details><summary>Valgrind output (first lines)</summary>" "" '```text'
    grep -v '^==[0-9]*== *$' "$LOGDIR/valgrind.txt" | head -n 60 | sed 's/```/'"'''"'/g' >>"$SUMMARY"
    say '```' "" "</details>" ""
fi
say "**Reproduce locally:** \`cd $TASK && make memcheck\` (needs \`sudo apt install valgrind\`)." ""
exit 0
