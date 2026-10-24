# GitHub setup (teacher)

Step by step in the GitHub web UI. Do this once, after the first push of the
repository to `github.com/adiber1/c-course`.

## 1. Keep the repository public

**Settings → General → Danger Zone → Change visibility**: it must stay
**Public**.

On GitHub Free, rulesets on a personal repository and unlimited free Actions
minutes only exist for public repositories. Because the repo is public,
reference solutions must never be committed. `solutions/` is in
`.gitignore`; before every commit, check with `git status` that nothing from
it is staged.

Students are **not** collaborators. Do not add them under *Settings →
Collaborators*. They work in forks.

## 2. Ruleset on `main`

Either run `scripts/setup-ruleset.sh` (needs `gh auth login`; read it first),
or click it together:

1. **Settings → Rules → Rulesets → New ruleset → New branch ruleset**.
2. **Ruleset name:** `protect-main`. **Enforcement status:** Active.
3. **Bypass list → Add bypass → Repository admin** (that is you), mode
   *Always*. This lets you push task fixes directly to `main`.
4. **Target branches → Add target → Include default branch.**
5. Rules:
   - ☑ **Restrict deletions**
   - ☑ **Block force pushes**
   - ☑ **Require a pull request before merging**: required approvals `0`
   - ☑ **Require status checks to pass**: **Add checks** → type `check` →
     select it (it appears after the first workflow run; you can also type it
     before that).
6. **Create**.

PRs are never merged, so the rules mainly protect `main` from accidents and
make the `check` result visible on every PR.

## 3. Actions settings

**Settings → Actions → General:**

1. **Actions permissions:** *Allow all actions and reusable workflows* (or
   allow GitHub-authored actions; the workflows only use `actions/*`).
2. **Fork pull request workflows from outside collaborators:** keep
   **Require approval for first-time contributors**. Each student's first PR
   shows *"1 workflow awaiting approval"*: open the PR → **Approve and run
   workflows**. You do this once per student.
3. **Workflow permissions:** leave the default (*Read repository contents and
   packages permissions*). Each workflow declares what it needs:
   - `check.yml`: `contents: read` only. It runs the student's code, so it
     never gets write access or secrets.
   - `score-comment.yml`: `pull-requests: write` and `actions: read`. It is
     started by `workflow_run`, runs the version from `main`, never checks out
     PR code, and only reads the score artifact.

### Confirm the comment workflow works on fork PRs

After the first student PR (or your own test PR from a second account or a
fork):

1. **Actions** tab → *Check* run for the PR → green or red, with a summary.
2. **Actions** tab → *Score comment* run, started right after it.
3. The PR shows **one** comment starting with "Score: NN / 100". Push again:
   the same comment is updated, no second comment appears.

If *Score comment* fails with a permission error, check that **Settings →
Actions → General → Workflow permissions** is not set to block GitHub Actions
from creating pull request comments, and that the workflow file on `main` is
unchanged.

## 4. Reviewing a PR and closing it after the defense

1. Open the PR. The **score comment** shows the total, the parts, the
   deductions and failing tests. **Checks → check → Summary** has the full
   report (warning texts, sanitizer locations).
2. **Files changed**: only the task's student files may appear. If anything
   else appears, the score comment says the result is not valid.
3. **Commits**: look at the history. Steady small commits are a good sign;
   one giant commit is a reason to ask more questions in the defense.
4. Hold the defense (see `defense-guide.md`).
5. Write a short comment (date, outcome), then **Close pull request**. Do
   **not** merge.

## 5. Updating a task later

Push to `main` directly (you are in the bypass list). Before pushing:

```bash
scripts/sync-solutions.sh      # copy the changed task files into solutions/ (keeps your solution files)
scripts/verify-all.sh          # every task: starter < 60, solution = 100, solutions/ up to date
git status                     # nothing from solutions/ may be staged
```

Each `solutions/NN-name/` is a complete copy of its task with the reference
solution in place, so you can also work there directly: `make test`,
`make score`.

`verify-all.sh` needs Linux (WSL, or Docker on a Mac), like CI. macOS's
clang has no leak detection and macOS has no `timeout` command, so results
there can differ. On a Mac:

```bash
docker run --rm --cap-add SYS_PTRACE -v "$PWD":/repo -w /repo ubuntu:24.04 bash -c \
  'apt-get update -qq && apt-get install -y -qq build-essential cppcheck rsync >/dev/null && scripts/verify-all.sh'
```

Students with an open PR on a changed task may need to run
`git merge upstream/main` (the student README explains it).

## Security model, in short

- A PR can change any file, including the workflow and the scripts. Two
  measures handle that:
  1. `check.yml` builds and scores inside a **trusted copy of `main`** and
     copies in only the task's `STUDENT_FILES` from the PR, so changed
     tests, Makefiles or scripts in the PR are never used for scoring.
  2. `score-comment.yml` always runs from `main` and independently lists the
     PR's files through the API. If anything outside `tasks/*/src/` and
     `tasks/*/ANSWERS.md` changed, the comment says the score is **not
     valid**, even if a modified `check.yml` reported success.
- The comment workflow treats the uploaded artifact as untrusted: size limits,
  strict validation of every number and name, and the PR number must belong
  to the PR whose head commit was checked.
- `check` runs student code with a read-only token and no secrets.
