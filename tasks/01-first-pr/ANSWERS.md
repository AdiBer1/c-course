# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
My local result depends on my own machine: my compiler version, my installed tools, and files that may exist only on my computer or were never committed. CI builds only what is actually in the pull request, on a clean machine that is the same for everyone, so a passing result there shows that the code works for anyone and not just for me. It also means the score cannot be changed or faked locally, which makes it a trustworthy, reproducible check.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
A test defines what "correct" means. If I could edit the tests, I could make them match my code instead of making my code match the requirements, and a passing score would prove nothing. Tests act as a fixed contract between the code and the rest of the team: whenever the code changes, they reveal whether something broke. For that to work they must stay independent of whoever wrote the code under test.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
C passes arguments by value, so the function receives its own copies. If the function changes `a`, only its local copy changes, and that change disappears when the function returns. The caller's variable keeps its original value, so the caller does not notice anything. To modify the caller's variable, the function would need a pointer (for example `int *a`), which gives access to the original memory location.