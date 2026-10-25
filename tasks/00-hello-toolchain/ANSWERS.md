# Task 00 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.
There is no code to write here; these are about *why*.

## Q1. `gcc -c mathx.c` produces `mathx.o`, but that file cannot run. What is missing, and which step adds it?

Answer: 
The `mathx.o` file cannot run because it lacks the `main` function, which is the entry point of the program. The step that adds it is the linking stage, where `mathx.o` is combined with `main.o` to create a final, complete executable file.

## Q2. What would go wrong if `mathx.h` had no include guard and some `.c` file ended up including it twice (directly or through another header)? Would it still go wrong if the header only contained function declarations?

Answer:
 If the file is included twice, all of its content is copied twice into the same source file. If it contained definitions (such as structs, data types, or global variables), the compiler would fail and throw a redefinition error. However, if the file contains only function declarations, no error will occur and the code will compile normally. This is because in C, you are allowed to declare the same function as many times as you want, as long as the declarations are completely identical.

## Q3. You change only `mathx.c`. Which files does your Makefile rebuild, and why is that better than recompiling everything — especially on a big firmware project?

Answer: 
The Makefile will recompile only the `mathx.c` file (to generate a new `mathx.o` object file), and then perform linking with the old `main.o` to create the updated `hello` executable. The `main.c` file will not be recompiled. This is better because in large firmware projects containing dozens or hundreds of files, recompiling the entire project from scratch takes a lot of time. Compiling only what has changed saves immense processing power — it is the difference between waiting 5 seconds and waiting 5 minutes every time you make a small change to the code.