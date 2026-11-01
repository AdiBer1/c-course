# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Local environments can differ, have leftover build artifacts, or contain uncommitted file changes that mask bugs. Running tests on a clean CI environment guarantees that the code is fully reproducible, compiles without environment-specific dependencies, and truly passes all tests on a neutral setup.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
Tests serve as the official specification and quality contract of the project. If a developer could modify the tests, they could alter or remove failing test cases to artificially pass the build, bypassing the actual requirements and compromising system reliability.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
In C, arguments are passed by value, meaning the function receives independent local copies of the caller's variables. If `max3` modifies `a`, only the internal copy inside the function changes; the caller's original variable remains completely unaffected.

