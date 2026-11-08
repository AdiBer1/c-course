# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
Because each computer may have different settings, and testing again checks if the code works in other environments too.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
Because we have to make sure that our code meets the requirements, instead of changing the tests to fit our code. The tests are there to show us what the requirements are and check if our code meets them.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
No, because in C the function receives only a copy of the value. If we change a inside the function, it does not change the original variable in the caller.

