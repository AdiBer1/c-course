# Tutor notes: task 00, Hello, toolchain

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Preprocess → compile → link; what a `.o` file contains and what it lacks
- Header = declarations (the interface), `.c` = definitions; include guards
- Make rules: target, prerequisites, recipe; why prerequisites must include headers

## Common mistakes and the guiding question for each

- **Recipe lines indented with spaces → `missing separator`.**  
  Ask: *What exact character does make require at the start of a recipe line?*
- **Compiling everything in one command (`gcc main.c mathx.c -o hello`).**  
  Ask: *Which files would have to be recompiled if you changed only `mathx.c`? How would make know?*
- **`.o` rules don't list `mathx.h`, so the header-change test fails.**  
  Ask: *If you edit `mathx.h`, which outputs are now out of date, and does your Makefile know that?*
- **Putting the function definitions into the header.**  
  Ask: *What happens at link time if two `.c` files include a header that defines a function?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: what are the two separate steps that turn `.c` files into a program?
2. Show the general rule shape: `target: prerequisites` + TAB + command.
3. Point to `gcc -c file.c -o file.o` for compiling and `gcc a.o b.o -o prog` for linking.
4. Pseudo-code: hello depends on main.o and mathx.o; each .o depends on its .c and on mathx.h.
5. Example on a different problem: a Makefile for `blink` from `blink.c` + `delay.c` + `delay.h`.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. Run `make` twice in `src/`. Why does the second run do nothing?
2. You delete `mathx.o` only. What exactly does `make` run now, and why?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-00`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Walk me through what `gcc -c mathx.c` produces and why that file cannot run.
2. Show me in your Makefile how make knows to rebuild after `mathx.h` changes.
3. What is an include guard protecting against? Show it in `mathx.h`.
