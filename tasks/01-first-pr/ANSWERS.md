# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
Every local machine is configured differently, and local test results can be modified. Running CI on a fresh machine ensures that the code builds and passes tests in a clean, standardized environment without relying on local machine quirks.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
Tests serve as the official requirements for the program. If we were allowed to change the tests, we could modify or delete failing test cases to get a passing score without actually fixing the code. Keeping tests immutable guarantees that everyone's implementation is evaluated fairly against the same rules.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
In C, function parameters are passed by value, meaning the function receives local copies of the caller's arguments. If max3 modifies a, only its local copy is changed; the original variable in the caller remains unchanged.

