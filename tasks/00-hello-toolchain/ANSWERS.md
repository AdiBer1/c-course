# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
What is missing: `mathx.o` only contains object code for the functions defined in `mathx.c`. It is missing the entry point function `main()`, as well as standard library runtime startup code.

Which step adds it: The **linking** step (e.g., `gcc -o hello main.o mathx.o`). The linker combines all object files (`main.o` and `mathx.o`) and resolves references to produce a single executable binary (`hello`).

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
If a header file lacks an include guard and is included twice in the same `.c` file, the preprocessor duplicates its content. If the header contains type definitions (`typedef`), structures (`struct`), macros, or variable definitions, this causes a compilation error due to redefinition / redeclaration of types.

However, if the header contains **only function declarations**, C allows declaring the same function multiple times as long as the signatures are identical. In that specific case, it would still compile without errors, though omitting include guards is bad practice because adding a `struct` or type definition later would immediately break the build.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
Files rebuilt: Only `mathx.o` is recompiled, and then the final binary `hello` is relinked (`main.o` is not rebuilt).

Why it is better: This enables **incremental builds**. In large firmware projects with hundreds of `.c` files, recompiling everything on every minor change can take minutes or hours. Make compares file modification timestamps and only rebuilds what changed (and what depends on it), reducing build time to seconds.
