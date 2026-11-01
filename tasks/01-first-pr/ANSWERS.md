# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
Running on a fresh CI environment ensures that the code builds and passes tests independently of local configurations, missing dependencies, or cached files on my personal laptop. It guarantees reproducibility and verifies that the code is truly robust and correct for anyone.
## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
The test suite serves as an objective, standardized specification and evaluation contract. If students could modify the tests, they could alter them to pass broken code rather than fixing the implementation itself, defeating the purpose of having a reliable and fair grading benchmark.
## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
Because parameters in C are passed by value, max3 receives temporary local copies of the caller's variables. If the function modifies a, it only affects its own local copy on the stack, so the caller's original variables remain completely unchanged and the caller will not notice any difference.

