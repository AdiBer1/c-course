# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
mathx.o has no function where the program can start running. It only contains the functions from mathx.c, translated into machine code. The starting point of the program is in main. After I compile main.c into main.o, it still does not know the implementation of these functions or where they are. That is why main.o and mathx.o must be linked together: the linking step (done by the linker, gcc main.o mathx.o -o hello) connects the separately compiled object files into one executable, hello, that can run.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
A header can contain definitions (for example, of variables) and declarations (for example, of functions). Something can be defined only once, but it can be declared more than once. Without an include guard, if a .c file includes the header twice, the compiler sees its contents twice. If the header contains a definition, the compiler sees that definition twice and stops with a compile error (“redefinition”). If the header contains only function declarations, as in our case, the duplicate declarations do not cause an error, so in our case it would still work.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:
It rebuilds mathx.o, which translates the updated code into machine code, and hello, so that the calls to the functions are linked to their updated implementations. This is the advantage of compiling in separate steps: I don’t need to recompile main.c. In short programs this doesn’t matter much, but in big projects with many small source files it can be a difference of several minutes for each change.

