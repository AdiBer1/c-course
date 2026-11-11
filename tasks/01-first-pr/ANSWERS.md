# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
My laptop can hide problems: an old file left in `build/`, a different compiler version, extra flags, or a change I forgot to commit. CI starts from a clean machine and uses only what is actually in the PR, so it proves the code works for everyone, not just on my computer. It is also a result nobody can fake or edit by hand, so the team (and the reviewer) can trust it.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
A test is the agreed definition of "correct" behavior, the specification written as code. If I could change the tests, I could make them pass by weakening them instead of fixing my code, and a passing result would mean nothing. Keeping the tests fixed means the code is checked against the same standard for every student, and a green CI really means the function does what the header promises.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
C passes arguments by value: when `max3` is called, the values are copied into new local variables `a`, `b`, `c` that exist only inside the function. If the function changes `a`, it changes only its own copy, and that copy disappears when the function returns. The caller's variable keeps its original value, so the caller does not notice. To let a function change the caller's variable you would have to pass a pointer to it.