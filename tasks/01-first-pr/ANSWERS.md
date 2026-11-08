# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each Answer: line.

## Q1. make score already gives you a score on your laptop. Why does CI build and test your code again on a fresh machine instead of trusting your local result?

Answer: Local environments can be inconsistent, and users could potentially modify test files or build configurations locally. Running CI on a fresh, controlled virtual machine ensures an objective, standardized, and secure evaluation of the code.

## Q2. Your PR may only change src/max3.c and ANSWERS.md. Why is it important that you cannot change the tests (think about what a test is for)?

Answer: Tests serve as the objective specification and standard of correctness. If developers were allowed to modify the tests, they could alter them to fit flawed code rather than fixing the code to meet the correct requirements.

## Q3. In max3, the parameters a, b, c are copies of the caller's values. What does that mean if your function changes a? Does the caller notice?

Answer: Because the parameters are passed by value, they are local copies inside the function. If the function modifies a, only the local variable changes, and the caller's original variables remain completely unaffected.
