# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `make score` already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer:
Because a local result only indicates that the code works on my specific computer—given my files, my version of the compiler, and my personal configuration settings. It might depend on factors not present in the repository, such as a file I forgot to commit or a library installed only on my machine. The CI system starts from a clean state, receiving only what I have included in the PR. it then builds everything from scratch using standardized compilation flags. If the tests pass there, it ensures that the build will also succeed for anyone else. Furthermore, a local score could potentially be incorrect or even falsified. Since the CI result is verified by the system itself, a reviewer can trust it.

## Q2. Your PR may only change `src/max3.c` and `ANSWERS.md`. Why is it important that you cannot change the tests (think about what a test is *for*)?

Answer:
Because a test serves as the agreed-upon definition of what is considered "correct." Furthermore, it is written based on the instructor's definition of correctness, not mine. If I were to modify the test, I might end up adjusting the test to fit my code - rather than adjusting my code to fit the test - which would result in the test passing even though the code itself is incorrect.

## Q3. In `max3`, the parameters `a`, `b`, `c` are copies of the caller's values. What does that mean if your function changes `a`? Does the caller notice?

Answer:
Since only the values of the variables are passed, and not the variables themselves,
the parameters inside the function receive these values but are separate variables
from the caller's. They exist only while the function runs. Changing `a` inside the
function changes only this local copy, so the caller does not notice: its variable
keeps its original value. The only thing the caller gets back from the function is
the return value.
