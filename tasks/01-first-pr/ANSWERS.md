# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
CI builds and tests the code on a fresh machine to make sure it works correctly in a clean environment. This helps ensure that the results do not depend on something specific to my computer, such as local settings or files.


## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
Tests are used to check if the code works correctly according to the requirements. If I could change the tests, I could make them pass without actually fixing my code. Keeping the tests unchanged makes sure that my solution is tested fairly.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
In C, arguments are passed by value, which means the function receives copies of the original values. If the function changes a, only the local copy changes, and the original variable in the caller remains unchanged.

