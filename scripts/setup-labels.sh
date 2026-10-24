#!/usr/bin/env bash
# setup-labels.sh - teacher tool: create the PR labels once.
#
#   scripts/setup-labels.sh                 (uses adiber1/c-course)
#   scripts/setup-labels.sh owner/repo
#
# Needs the GitHub CLI, logged in as the repo owner: gh auth login
# Creates (or updates, thanks to --force):
#   task-00 .. task-21           which task the PR is for     (set by CI)
#   passed / needs-fix           CI status of the submission  (set by CI)
#   defended                     oral defense done            (set by YOU, by hand)
# score-comment.yml only adds and removes these labels; it never creates them.

set -euo pipefail

REPO=${1:-adiber1/c-course}
ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

command -v gh >/dev/null || { echo "Install the GitHub CLI first: https://cli.github.com" >&2; exit 1; }

for dir in "$ROOT"/tasks/*/; do
    name=$(basename "$dir")
    num=${name%%-*}
    gh label create "task-$num" --repo "$REPO" --force --color "1d76db" --description "Task $name"
done

gh label create passed    --repo "$REPO" --force --color "0e8a16" --description "CI score 60+: ready for the defense"
gh label create needs-fix --repo "$REPO" --force --color "d93f0b" --description "Below 60 or not scored: keep working"
gh label create defended  --repo "$REPO" --force --color "5319e7" --description "Oral defense done (set by the teacher)"

echo "Done. Labels: Issues > Labels (or Pull requests > Labels)."
