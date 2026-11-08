# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
The mathx.o file is an object file, not a complete executable program. It still needs to be linked with the other required object files and libraries to create an executable file. The linking step, usually done by gcc without the -c option, adds the missing parts and produces the final executable.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
Without an include guard, the contents of mathx.h would be processed twice when the header is included more than once. This can cause redefinition errors if the header contains things such as structs, typedefs, or other definitions. If the header contains only function declarations, including it twice normally does not cause a compilation error, because repeated function declarations are allowed as long as they are compatible.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
If only mathx.c changes, the Makefile should rebuild mathx.o and then relink the executable using the updated object file. It does not need to recompile unrelated source files whose dependencies have not changed. This saves time and computing resources, which is especially important in a large firmware project with many source files.
