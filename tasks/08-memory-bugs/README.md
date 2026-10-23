# Task 08: Hunting memory bugs

Stage 3, Memory.

## What you build

Find and fix the **6 bugs** in a given sensor-log module, using AddressSanitizer, UBSan, the compiler and the tests.

## Why this matters in embedded

Memory bugs are the hardest bugs in firmware: an off-by-one write silently corrupts the variable next to it, a dangling pointer works until an interrupt reuses that stack area, and the crash happens far away, hours later, on one board out of a hundred. On a PC you have tools that catch them at the exact line. Learning to read their reports is one of the most valuable debugging skills you will have.

## The API

Header `src/sensor_log.h` (given): a fixed-size log of `int16_t` readings in a static array.

`src/sensor_log.c` (given, **buggy**): this is the file you fix.

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

How to hunt:

1. `make test` and read the **first** sanitizer report: the kind (`global-buffer-overflow`, `stack-use-after-return`, ...), the line, and the stack trace.
2. Read the failing test's name and message.
3. `make` and read the compiler warnings: some point straight at a bug.
4. Fix one bug, run again. Repeat.

## Constraints

- Fix the existing code; do not rewrite the module from scratch. Change as little as needed.
- No `malloc`/`free` (stage 3 rule).
- The log must keep using its static array.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/08-memory-bugs/src/sensor_log.c`
- `tasks/08-memory-bugs/ANSWERS.md`

## How to run

Inside `tasks/08-memory-bugs/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-08`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
