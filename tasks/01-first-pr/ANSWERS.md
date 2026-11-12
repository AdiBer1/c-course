# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
Local environments may have untracked dependencies, leftover build artifacts, or custom configurations that cause code to pass locally even if it is broken ("works on my machine" syndrome). 

CI runs tests on a standardized, clean machine to guarantee that the build is completely reproducible, uses only the repo's actual contents, and passes all tests independently before merging into main.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
Tests serve as the independent contract and specification for how the code must behave. If a developer or PR could modify the tests, they could alter the expected results or delete failing test cases to make buggy code pass. 

Restricting changes strictly to source and answer files ensures that code is objectively validated against immutable requirements.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
In C, arguments are passed by value, meaning the function receives independent local copies of `a`, `b`, and `c` on the stack. 

If `max3` modifies `a`, it only changes its own local copy. The caller's original variable remains completely unaffected and unmodified.

