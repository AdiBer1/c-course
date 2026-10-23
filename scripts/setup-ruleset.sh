#!/usr/bin/env bash
# setup-ruleset.sh - teacher tool: protect `main` with a repository ruleset.
#
#   scripts/setup-ruleset.sh                 (uses adiber1/c-course)
#   scripts/setup-ruleset.sh owner/repo
#
# Needs the GitHub CLI, logged in as the repo owner: gh auth login
# Creates (or updates) a ruleset "protect-main" on the default branch:
#   - changes only through a pull request
#   - the "check" status check must pass
#   - no force pushes, no deletion
#   - repository admins (you) may bypass, so you can push task updates directly
#
# This only describes the ruleset; read it before running. Nothing else is changed.

set -euo pipefail

REPO=${1:-adiber1/c-course}
NAME="protect-main"

command -v gh >/dev/null || { echo "Install the GitHub CLI first: https://cli.github.com" >&2; exit 1; }

BODY=$(cat <<JSON
{
  "name": "$NAME",
  "target": "branch",
  "enforcement": "active",
  "conditions": {
    "ref_name": { "include": ["~DEFAULT_BRANCH"], "exclude": [] }
  },
  "bypass_actors": [
    { "actor_id": 5, "actor_type": "RepositoryRole", "bypass_mode": "always" }
  ],
  "rules": [
    { "type": "deletion" },
    { "type": "non_fast_forward" },
    {
      "type": "pull_request",
      "parameters": {
        "required_approving_review_count": 0,
        "dismiss_stale_reviews_on_push": false,
        "require_code_owner_review": false,
        "require_last_push_approval": false,
        "required_review_thread_resolution": false
      }
    },
    {
      "type": "required_status_checks",
      "parameters": {
        "strict_required_status_checks_policy": false,
        "required_status_checks": [ { "context": "check" } ]
      }
    }
  ]
}
JSON
)

existing=$(gh api "repos/$REPO/rulesets" --jq ".[] | select(.name == \"$NAME\") | .id" || true)
if [ -n "$existing" ]; then
    echo "Updating ruleset $NAME (id $existing) on $REPO"
    gh api --method PUT "repos/$REPO/rulesets/$existing" --input - <<<"$BODY" >/dev/null
else
    echo "Creating ruleset $NAME on $REPO"
    gh api --method POST "repos/$REPO/rulesets" --input - <<<"$BODY" >/dev/null
fi
echo "Done. Check it in Settings > Rules > Rulesets."
