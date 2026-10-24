#!/usr/bin/env bash
# format-check.sh - report (never change) clang-format differences.
#
#   scripts/format-check.sh <task-dir>               all student .c/.h files
#   scripts/format-check.sh <task-dir> src/a.c ...   only these student files
#
# Uses the repository's .clang-format. Informational: always exits 0 unless
# called wrongly. Writes a report to $GITHUB_STEP_SUMMARY (stdout locally)
# and, inside GitHub Actions, one annotation per file.

set -uo pipefail

TASK=${1:?usage: format-check.sh <task-dir> [files...]}
TASK=${TASK%/}
shift
SUMMARY=${GITHUB_STEP_SUMMARY:-/dev/stdout}
say() { printf '%s\n' "$@" >>"$SUMMARY"; }

student=$(make -s --no-print-directory -C "$TASK" config 2>/dev/null | sed -n 's/^STUDENT_FILES=//p')
is_student() { local f; for f in $student; do [ "$f" = "$1" ] && return 0; done; return 1; }

files=()
if [ $# -gt 0 ]; then
    for f in "$@"; do is_student "$f" && files+=("$f"); done
else
    for f in $student; do files+=("$f"); done
fi
c_files=()
for f in ${files[@]+"${files[@]}"}; do
    case $f in *.c | *.h) [ -f "$TASK/$f" ] && c_files+=("$f") ;; esac
done

if ! command -v clang-format >/dev/null 2>&1; then
    say "### 🎨 Formatting: NOT RUN" "" "clang-format is not installed (\`sudo apt install clang-format\`)."
    exit 0
fi
version=$(clang-format --version | head -n1)

if [ ${#c_files[@]} -eq 0 ]; then
    say "### 🎨 Formatting: nothing to check" "" "No changed C files of yours."
    exit 0
fi

bad=()
diffs=$(mktemp)
for f in "${c_files[@]}"; do
    if ! (cd "$TASK" && clang-format "$f" | diff -u --label "$f" --label "$f (formatted)" "$f" -) >>"$diffs"; then
        bad+=("$f")
        first=$(cd "$TASK" && clang-format "$f" | diff "$f" - | sed -n '1s/^\([0-9]*\).*/\1/p')
        if [ "${GITHUB_ACTIONS:-false}" = true ]; then
            echo "::notice file=$TASK/$f,line=${first:-1},title=Formatting (informational)::This file does not match the course style. See the Formatting check summary, or run: clang-format -i $f"
        fi
    fi
done

if [ ${#bad[@]} -eq 0 ]; then
    say "### 🎨 Formatting: PASSED ✅" "" "Checked with \`$version\`: ${c_files[*]}" ""
else
    say "### 🎨 Formatting: differences found ℹ️ (informational)" ""
    say "**Does not affect your grade.** Consistent formatting makes code easier to read and review, as in any professional team." ""
    say "Files that differ from the course style (\`.clang-format\`, $version): $(printf '`%s` ' "${bad[@]}")" ""
    say "<details><summary>Suggested changes</summary>" "" '```diff'
    head -n 200 "$diffs" | sed 's/```/'"'''"'/g' >>"$SUMMARY"
    say '```' "" "</details>" ""
    say "**Fix locally:** \`cd $TASK && clang-format -i ${bad[*]}\` (look at the diff before you commit)." ""
fi
rm -f "$diffs"
exit 0
