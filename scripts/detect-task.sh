#!/usr/bin/env bash
# detect-task.sh - which single task does this change touch?
#
#   scripts/detect-task.sh <base> [<head>]      (head defaults to HEAD)
#
# Compares <head> with its merge-base with <base>. Prints the task folder
# (e.g. "tasks/07-command-parser") and exits 0 when exactly one task folder
# is touched. Otherwise explains the problem and exits 1.
#
# If GUARD_JSON is set, a machine-readable result is written to that file
# (used by CI to build the PR comment).

set -uo pipefail

BASE=${1:?usage: detect-task.sh <base> [<head>]}
HEAD=${2:-HEAD}

json_str() { printf '%s' "$1" | tr -d '\000-\037' | sed 's/\\/\\\\/g; s/"/\\"/g'; }
write_json() {   # reason, then file list
    [ -n "${GUARD_JSON:-}" ] || return 0
    local reason=$1; shift
    {
        printf '{"version":1,"step":"detect-task","ok":%s,"reason":"%s","files":[' \
            "$([ "$reason" = ok ] && echo true || echo false)" "$reason"
        local sep=""
        for f in "$@"; do printf '%s"%s"' "$sep" "$(json_str "$f")"; sep=","; done
        printf ']}\n'
    } >"$GUARD_JSON"
}

if ! changed=$(git diff --name-only --no-renames "$BASE...$HEAD" 2>/dev/null); then
    echo "detect-task: cannot compare $BASE with $HEAD" >&2
    exit 2
fi

tasks=$(printf '%s\n' "$changed" | sed -n 's#^tasks/\([^/]*\)/.*#\1#p' | sort -u)
n=$(printf '%s' "$tasks" | grep -c . || true)

if [ "$n" -eq 1 ]; then
    write_json ok "tasks/$tasks"
    echo "tasks/$tasks"
    exit 0
fi

if [ "$n" -eq 0 ]; then
    echo "This pull request does not change any task folder."
    echo "One PR = exactly one task: change only the files of tasks/NN-name/."
    if [ -n "$changed" ]; then
        echo "Files changed:"
        printf '%s\n' "$changed" | sed 's/^/  - /'
    fi
    # shellcheck disable=SC2086
    write_json no_task $changed
else
    echo "This pull request touches $n task folders. One PR = exactly one task."
    echo "Tasks touched:"
    printf '%s\n' "$tasks" | sed 's/^/  - tasks\//'
    echo "Make one branch per task (task-NN) and open a separate PR for each."
    # shellcheck disable=SC2086
    write_json multiple_tasks $(printf '%s\n' "$tasks" | sed 's#^#tasks/#')
fi
exit 1
