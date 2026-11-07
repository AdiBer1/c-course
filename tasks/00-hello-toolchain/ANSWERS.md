# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer: 
mathx.o contains machine code for square and clamp only. It has no main (no entry point), and any references to code outside it are still unresolved. The linker combines all the .o files and libraries into one executable and resolves those references. In our Makefile this is the hello rule.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer: 
Without an include guard, if a file is included twice, its entire content is duplicated. Duplicate struct or enum definitions (or a variable definition with an initializer) will cause a compilation error, because the same thing is defined twice. In contrast, if the header contains only function declarations, there is no error, because C allows the same function to be declared more than once as long as the declarations match. Nevertheless, an include guard is still important, because header files tend to grow over time and may later contain definitions.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer: 
If only `mathx.c` is modified, the system will rebuild only `mathx.o` and then re-run the linking stage to generate the executable file `hello`. The file `main.o` remains unchanged. The build system determines this by checking the files' timestamps (i.e., when they were last modified). In a large-scale project containing hundreds of files, this distinction can mean the difference between a build process that takes a few seconds and one that takes several minutes.

