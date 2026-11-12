## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer:
Answer: `mathx.o` is only a compiled object file. It contains machine code for the functions in `mathx.c`, but it has no entry point (`main`), and references to code defined in other files (or in the C library) are still unresolved. The linking step adds what is missing: the linker combines `main.o` and `mathx.o` (and the needed library code, like the C runtime startup code), resolves the references between them, and produces the executable `hello`. In my Makefile this is the `gcc -o hello main.o mathx.o` command.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
Answer: Without an include guard, the preprocessor would paste the contents of `mathx.h` into the file twice. If the header contains definitions, such as a struct or typedef definition, an enum, or a function body, the compiler reports a redefinition error (for example "redefinition of struct X"). If the header contains only function declarations (prototypes), it would usually still compile, because repeating an identical function declaration is legal in C. But it is still bad practice: as soon as someone adds a type or inline function to the header, it breaks. An include guard (`#ifndef MATHX_H` / `#define MATHX_H` / `#endif`, or `#pragma once`) makes the header safe to include any number of times.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer:

Answer: Only `mathx.o` is recompiled (because `mathx.c` is newer than it), and then `hello` is relinked (because `mathx.o` is newer than it). `main.o` is not rebuilt, since neither `main.c` nor `mathx.h` changed. This is better than recompiling everything because compilation is the slow part: on a big firmware project with hundreds of source files, rebuilding only the changed file and relinking saves a lot of time, so the edit-build-test cycle stays fast.