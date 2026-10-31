# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
The `-c` flag tells the compiler to perform compilation only, creating an object file (`mathx.o`) containing machine code without creating an executable. It is missing the linking step (performed by the linker, `ld`, or `gcc` without `-c`), which combines object files, resolves function references, and links the entry point (`main` function) and startup code to create a runnable binary.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
Without include guards, multiple inclusions cause the preprocessor to duplicate the contents of the header file, which can lead to redefinition errors for types, macros, or structs. However, if the header only contains function declarations, C allows multiple identical function declarations in a single compilation unit, so it would compile successfully—though it remains bad practice.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
The Makefile will recompile only `mathx.c` into `mathx.o` and then re-link the final executable (`hello`). It avoids recompiling unchanged files like `main.c`. This incremental compilation dramatically speeds up build times, which is essential for large firmware projects with thousands of files where full rebuilds could take hours.
