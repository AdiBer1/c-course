# Tutor notes: task 01, Your first pull request

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Pass-by-value
- The fork/branch/PR/CI loop
- Reading a CI score report

## Common mistakes and the guiding question for each

- **Committing to `main` of the fork instead of a `task-01` branch.**  
  Ask: *Which branch is your PR coming from? What does the README say it should be called?*
- **Changing the test file or another task's files: the check fails with "not allowed".**  
  Ask: *What does the CI message list as not allowed, and how do you undo that change?*
- **Using `>=`/`>` inconsistently and failing the equal-values test.**  
  Ask: *What should `max3(4, 4, 1)` return, and does every branch of your code handle a tie?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask how she would find the largest of three numbers by hand.
2. Concept: keep the best value seen so far, compare it with each other value.
3. Point to the line that handles ties or negative numbers if a test fails there.
4. Pseudo-code: largest = a; if b > largest: largest = b; same for c.
5. Example on a different problem: the smallest of three numbers.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What happens to the caller's variable if `max3` changes `a`?
2. Which file would the CI reject if you edited `tests/test_max3.c`, and why?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-01`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Show me your PR: which commits, which branch, what did CI report?
2. Why can't a PR change the tests?
