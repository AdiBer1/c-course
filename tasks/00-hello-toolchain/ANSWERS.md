# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
`mathx.o` is an unlinked object file containing compiled machine code, but it lacks the program's entry point (`main`), standard runtime startup code (such as CRT/glibc initialization), and resolved external symbol references. The linking step (`gcc ... -o hello`) combines the object files, resolves function addresses, and adds the required startup logic to produce a runnable executable.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
If the header contains type definitions (`struct`, `enum`, `typedef`), including it twice without include guards causes a compilation error due to duplicate/redefined types. However, if the header contains only function declarations (prototypes), C permits repeated identical declarations within the same translation unit, so the compiler will not fail, though it wastes preprocessing time.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
The Makefile only recompiles `mathx.c` to produce `mathx.o`, and then relinks `mathx.o` with the existing `main.o` to recreate the binary `hello` (`main.c` is not recompiled). In a large embedded/firmware project with hundreds of files, rebuilding only the modified module and relinking takes seconds instead of minutes, drastically shortening the development and feedback cycle.