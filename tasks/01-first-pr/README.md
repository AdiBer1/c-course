# Task 01: Your first pull request

Stage 0, Toolchain.

## What you build

One tiny function, `max3`, delivered through the full fork → branch → PR → CI → defense flow.

## Why this matters in embedded

In a firmware team nobody pushes straight to the main branch: every change is a pull request that CI builds and tests before a human reviews it. Learning that flow now, on a 10-line function, means that from task 02 on you only have to think about C.

## The API

Header `src/max3.h` (given):

- `int max3(int a, int b, int c);` returns the largest of the three.

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

The goal of this task is the **workflow**. Go through every step in the root README: create the branch, commit, push, open the PR, read the CI comment, push a fix if needed, and watch the comment update.

## Constraints

- Change only `src/max3.c` and `ANSWERS.md`.
- Follow the PR steps in the root README exactly: branch `task-01`, commit, push, open the PR.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/01-first-pr/src/max3.c`
- `tasks/01-first-pr/ANSWERS.md`

## How to run

Inside `tasks/01-first-pr/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-01`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
