#!/usr/bin/env bash
# verify-all.sh - teacher tool: check every task against the local reference
# solutions in solutions/ (which are never committed).
#
#   scripts/verify-all.sh            all tasks
#   scripts/verify-all.sh 07 14      only tasks whose folder starts with 07 or 14
#
# For each task, in a temporary copy of the repo:
#   1. the untouched starter must score BELOW 60
#   2. with the solution files copied over it, the task must score exactly 100
#   3. the solution may only contain files listed in STUDENT_FILES

set -uo pipefail

ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
SOL="$ROOT/solutions"
[ -d "$SOL" ] || { echo "No solutions/ folder: reference solutions are local only." >&2; exit 2; }

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
rsync -a --exclude .git --exclude solutions --exclude build "$ROOT/" "$TMP/repo/"

score_of() { sed -n 's/^  "total": \([0-9]*\),$/\1/p' "$1/build/score.json"; }

failures=0
printf '%-24s %8s %9s  %s\n' TASK STARTER SOLUTION RESULT
for task_dir in "$ROOT"/tasks/*/; do
    name=$(basename "$task_dir")
    if [ $# -gt 0 ]; then
        match=0
        for f in "$@"; do case $name in "$f"*) match=1 ;; esac; done
        [ $match = 1 ] || continue
    fi
    work="$TMP/repo/tasks/$name"
    problems=()

    # 1. starter
    bash "$TMP/repo/scripts/score.sh" "$work" >/dev/null 2>"$TMP/err.txt" || problems+=("score.sh failed on starter: $(head -1 "$TMP/err.txt")")
    starter=$(score_of "$work"); starter=${starter:-?}
    [[ $starter =~ ^[0-9]+$ ]] && [ "$starter" -lt 60 ] || problems+=("starter scores $starter (must be < 60)")

    # 3. solution files must be student files
    allowed=$(make -s --no-print-directory -C "$work" config | sed -n 's/^STUDENT_FILES=//p')
    if [ ! -d "$SOL/$name" ]; then
        problems+=("no solution in solutions/$name")
        solution="-"
    else
        while IFS= read -r f; do
            rel=${f#"$SOL/$name/"}
            case " $allowed " in *" $rel "*) ;; *) problems+=("solution file $rel is not in STUDENT_FILES") ;; esac
        done < <(find "$SOL/$name" -type f ! -name '.DS_Store')

        # 2. solution
        rsync -a --exclude .DS_Store "$SOL/$name/" "$work/"
        bash "$TMP/repo/scripts/score.sh" "$work" >"$TMP/solution.md" 2>&1 || problems+=("score.sh failed on solution")
        solution=$(score_of "$work"); solution=${solution:-?}
        [ "$solution" = 100 ] || problems+=("solution scores $solution (must be 100)")
    fi

    if [ ${#problems[@]} -eq 0 ]; then
        printf '%-24s %8s %9s  %s\n' "$name" "$starter" "$solution" ok
    else
        printf '%-24s %8s %9s  %s\n' "$name" "$starter" "$solution" FAIL
        for p in "${problems[@]}"; do echo "    - $p"; done
        if [ "$solution" != 100 ] && [ -f "$TMP/solution.md" ]; then sed 's/^/      | /' "$TMP/solution.md" | head -40; fi
        failures=$((failures + 1))
    fi
    # restore the pristine task for the next run
    rm -rf "$work"; rsync -a --exclude build "$ROOT/tasks/$name/" "$work/"
done

echo
if [ $failures -eq 0 ]; then echo "All tasks OK."; else echo "$failures task(s) FAILED."; fi
[ $failures -eq 0 ]
