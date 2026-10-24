#!/usr/bin/env bash
# sync-solutions.sh - teacher tool: make every solutions/NN-name/ a complete,
# runnable copy of its task folder.
#
#   scripts/sync-solutions.sh            all tasks
#   scripts/sync-solutions.sh 07 14      only tasks whose folder starts with 07 or 14
#
# Everything from tasks/NN-name/ is copied into solutions/NN-name/ EXCEPT the
# task's STUDENT_FILES: those keep their solution version. Run it after you
# change a task (header, tests, task.mk, ...). Then `make test` / `make score`
# work directly inside solutions/NN-name/.
#
# solutions/ is git-ignored and must never be committed.

set -euo pipefail

ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
SOL="$ROOT/solutions"
mkdir -p "$SOL"

for task_dir in "$ROOT"/tasks/*/; do
    name=$(basename "$task_dir")
    if [ $# -gt 0 ]; then
        match=0
        for f in "$@"; do case $name in "$f"*) match=1 ;; esac; done
        [ $match = 1 ] || continue
    fi
    students=$(make -s --no-print-directory -C "$task_dir" config | sed -n 's/^STUDENT_FILES=//p')
    excludes=(--exclude build --exclude .DS_Store --exclude '*.o' --exclude /src/hello)
    for f in $students; do excludes+=(--exclude "/$f"); done

    mkdir -p "$SOL/$name"
    # --delete removes given files that no longer exist in the task, but the
    # excluded student files are left alone.
    rsync -a --delete "${excludes[@]}" "$task_dir" "$SOL/$name/"

    missing=""
    for f in $students; do [ -e "$SOL/$name/$f" ] || missing="$missing $f"; done
    if [ -n "$missing" ]; then
        echo "$name: synced, but the solution is missing:$missing"
    else
        echo "$name: synced"
    fi
done
