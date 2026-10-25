# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
While the file contains machine code, it is not necessarily a complete program. Sometimes, a linker is required to combine it with other necessary files.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
Without this mechanism (an "include guard"), a header file might appear in multiple places, leading to duplicate definitions or declarations that could cause compilation errors. This mechanism prevents the contents of a header file from being processed more than once.
## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
We will check based on dependencies.
`mathx.o` must be rebuilt, because it depends on `mathx.c`.
`1hello` must be recreated, because it depends on `mathx.o`.
