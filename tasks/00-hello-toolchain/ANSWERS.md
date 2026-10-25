# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer: 
`mathx.o` is an object file containing compiled machine code, but it is missing an entry point (the `main` function) and any necessary runtime or standard library functions. The **linking step** (done via `gcc -o hello ...`) combines `mathx.o` with `main.o` and resolves all references to create a complete, runnable executable.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer: 
Without an include guard, including the header multiple times can cause redefinition errors if the header contains type definitions, structs, enums, or macros. If the header *only* contains function declarations (prototypes), C actually permits multiple identical declarations, so it might still compile successfully; however, lacking guards is bad practice and will break as soon as any type definitions or global variables are added.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer: 
Your Makefile will rebuild only `mathx.o` (by recompiling `mathx.c`) and then re-link the final `hello` executable. `main.o` is untouched because its dependency (`main.c`) did not change. On a large firmware project with hundreds of source files, this incremental compilation saves massive amounts of time—turning a build process from minutes into seconds.

