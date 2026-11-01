# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
The mathx.o file contains compiled machine code, but it is not a complete executable program. It is missing the linking step, which combines the object files and connects function calls to their implementations. The linker creates the final executable file, hello.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
Without an include guard, including the same header multiple times may cause duplicate declarations or definitions, which can lead to compiler errors. If the header only contains function declarations, including it more than once is usually allowed in C, but include guards are still useful because they prevent repeated inclusion and protect against future problems.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
If I change only mathx.c, the Makefile rebuilds mathx.o and then links main.o and mathx.o again to create hello. It does not need to rebuild main.o because main.c and mathx.h have not changed. This saves compilation time, especially in large embedded projects with many source files.
