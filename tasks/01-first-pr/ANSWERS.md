# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
A program that works on my computer is not guaranteed to work the same way on every other computer (for example, a file that exists on my machine but was not committed, or a different compiler version). In addition, the teacher has no way to verify that I actually ran the tests and did not get the score some other way. CI builds and tests exactly the code in the PR, on a clean machine.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
The tests were written according to the behavior the system is supposed to have. If I could change them, I might change a test just to get a better score, instead of fixing my code to be correct. Then the test would pass, but the code would still be wrong. Tests are only meaningful if they check my code against the specification and I cannot change them.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
The caller does not notice the change, because the function does not receive the original variable, only a copy of its value. Even if I change a inside the function, the caller’s variable keeps its original value. The caller might only see a different return value, if the function uses the changed value to compute its result.

