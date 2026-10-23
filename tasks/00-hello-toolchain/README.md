# Task 00: Hello, toolchain

Stage 0, Toolchain.

## What you build

A two-file C program (`main.c` + `mathx.c`) that shares one header, built by a Makefile **you** write.

## Why this matters in embedded

Every firmware project is dozens or hundreds of `.c` files compiled separately and linked into one image that is flashed to the chip. A cross-compiler such as `arm-none-eabi-gcc` works exactly like the `gcc` on your PC: compile each file to an object file, then link. Build systems (Make, CMake) exist so that changing one file rebuilds only what depends on it; on a big project that is the difference between 5 seconds and 5 minutes per change.

## The API

Header `src/mathx.h` (given):

- `int square(int x);`
- `int clamp(int x, int lo, int hi);`

`src/main.c` (given) calls both and prints the results.

You write:

1. `src/mathx.c`: the two functions.
2. `src/Makefile`: your own small Makefile that, run inside `src/`, builds the program `hello`:
   - each `.c` is compiled **separately** to a `.o` (`gcc -c ...`), then the `.o` files are **linked**
   - the `.o` files depend on `mathx.h`, so editing the header rebuilds them
   - `make clean` removes `hello` and the `.o` files

Try it yourself: `cd src && make && ./hello`.

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

The task folder also has its own `Makefile` (the one that includes `../../common.mk`). That one builds and runs the **tests**; it is not yours to change. Your Makefile lives in `src/`.

## Constraints

- Do not change `mathx.h` or `main.c`.
- Your Makefile must use separate compile and link steps (the tests check that `main.o` and `mathx.o` exist).
- Remember: a Makefile recipe line starts with a **TAB**.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/00-hello-toolchain/src/mathx.c`
- `tasks/00-hello-toolchain/src/Makefile`
- `tasks/00-hello-toolchain/ANSWERS.md`

## How to run

Inside `tasks/00-hello-toolchain/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-00`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
