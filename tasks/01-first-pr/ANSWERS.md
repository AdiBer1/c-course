# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
CI builds and tests the code on a clean, isolated virtual machine rather than trusting the local result to ensure consistency, eliminate environment-specific issues (like local settings or leftover cache files), and provide an objective, standardized verification that the code compiles and passes all tests independently on any machine.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
A test serves as an objective, independent contract or specification of correct behavior. If developers could modify the tests themselves, they could alter or weaken them to fit faulty or incomplete code rather than fixing the implementation, which would defeat the entire purpose of automated quality assurance.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
When parameters are passed by value, the function receives a local copy of each argument. If the function modifies `a`, it only updates this internal local copy, while the caller's original variable remains completely unaffected and unchanged.
