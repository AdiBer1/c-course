# Task 01 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why do we create a new branch for every task instead of working directly on `main`?

Answer:
Creating a separate branch isolates changes for a specific feature or bug fix. It prevents untested code from breaking the stable `main` branch, enables clean Pull Requests for CI testing and code reviews, and allows multiple tasks to be developed in parallel without branch conflicts.

## Q2. What does CI (Continuous Integration) do when you open a Pull Request?

Answer:
CI automatically triggers automated workflows upon PR submission to build the code, run test suites, check for compiler warnings and static analysis findings, and verify that the proposed changes touch only allowed files before merging.