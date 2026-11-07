# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
`mathx.o` contains machine code for only one source file. It has no entry point (`main` and the startup code that calls it), and the calls to functions defined elsewhere (other files or the standard library, like `printf`) are still unresolved placeholders. Its addresses are also not final, because nothing has decided where each piece will sit in the finished program. The linking step adds all of this: the linker combines all the `.o` files and libraries, resolves every call to its definition, assigns final addresses, and produces one executable (`hello`). In embedded work, the linker output is the image that gets flashed to the chip.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
Without a guard, the header's contents would be pasted twice into the same translation unit. If the header contained definitions such as a `struct`, `typedef` or `enum`, the compiler would report a redefinition error. If the header contains only function declarations, it would still compile, because C allows repeating a declaration as long as the declarations are identical. Even so, a guard is good practice: the header may later gain a `struct` or `typedef`, at which point the error would suddenly appear, and repeated inclusion also wastes compile time.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
Only `mathx.o` is recompiled, because `mathx.c` is newer than it, and then `hello` is relinked, because one of its object files changed. `main.o` is not rebuilt, since neither `main.c` nor `mathx.h` changed. Make compares file timestamps and runs only the steps that are out of date. On a big firmware project with hundreds of files, a full rebuild can take minutes while an incremental build takes seconds, and that saving repeats on every small edit during the write-build-test cycle.