#!/usr/bin/env bash
# check-allowed-files.sh - does this change touch only the task's STUDENT_FILES?
#
#   scripts/check-allowed-files.sh <task-dir> <base> [<head>]
#
# <task-dir> is e.g. tasks/07-command-parser. STUDENT_FILES is read from that
# task's task.mk (in CI: from the trusted base version, not from the PR).
# Exits 0 when every changed file is allowed; otherwise lists the offending
# files and exits 1. Writes GUARD_JSON like detect-task.sh when it is set.

set -uo pipefail

TASK_DIR=${1:?usage: check-allowed-files.sh <task-dir> <base> [<head>]}
BASE=${2:?usage: check-allowed-files.sh <task-dir> <base> [<head>]}
HEAD=${3:-HEAD}
TASK_DIR=${TASK_DIR%/}

json_str() { printf '%s' "$1" | tr -d '\000-\037' | sed 's/\\/\\\\/g; s/"/\\"/g'; }
write_json() {
    [ -n "${GUARD_JSON:-}" ] || return 0
    local reason=$1; shift
    {
        printf '{"version":1,"step":"check-allowed-files","ok":%s,"reason":"%s","files":[' \
            "$([ "$reason" = ok ] && echo true || echo false)" "$reason"
        local sep=""
        for f in "$@"; do printf '%s"%s"' "$sep" "$(json_str "$f")"; sep=","; done
        printf ']}\n'
    } >"$GUARD_JSON"
}

# The task settings always come from the repo this script lives in. In CI that
# is the trusted copy of main, so a PR cannot widen its own STUDENT_FILES.
ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
[ -f "$ROOT/$TASK_DIR/task.mk" ] || { echo "check-allowed-files: $TASK_DIR has no task.mk" >&2; exit 2; }
allowed=$(make -s --no-print-directory -C "$ROOT/$TASK_DIR" config | sed -n 's/^STUDENT_FILES=//p')

if ! changed=$(git diff --name-only --no-renames "$BASE...$HEAD" 2>/dev/null); then
    echo "check-allowed-files: cannot compare $BASE with $HEAD" >&2
    exit 2
fi

bad=()
while IFS= read -r f; do
    [ -n "$f" ] || continue
    ok=0
    for a in $allowed; do
        [ "$f" = "$TASK_DIR/$a" ] && ok=1 && break
    done
    [ $ok = 1 ] || bad+=("$f")
done <<<"$changed"

if [ ${#bad[@]} -eq 0 ]; then
    write_json ok
    echo "OK: only allowed files changed."
    exit 0
fi

echo "This pull request changes files you are not allowed to change."
echo "For $(basename "$TASK_DIR") you may only change:"
for a in $allowed; do echo "  - $TASK_DIR/$a"; done
echo "Not allowed:"
for f in "${bad[@]}"; do echo "  - $f"; done
echo "Undo those changes (e.g. git checkout upstream/main -- <file>), commit and push again."
write_json forbidden_files "${bad[@]}"
exit 1
