# Task 18: Function pointers (a gentle intro)

Stage 6, Separating logic from hardware.

## What you build

Call functions through pointers: pass behaviour as an argument, look operations up in a table, and sort with a comparator.

## Why this matters in embedded

Function pointers are everywhere in embedded C, often without being called that: the interrupt vector table is an array of function pointers, command dispatchers map a name to a handler, RTOS tasks start from a function pointer, and driver interfaces (next task) are structs of function pointers. This task introduces the idea slowly, one use at a time.

## The API

Header `src/ops.h` (given), in three parts:

1. `op_add`, `op_sub`, `op_mul`, `op_max`, `apply_op`, `map_i32`
2. `OP_TABLE` (you fill it), `op_lookup`, `calc`
3. `cmp_ascending`, `cmp_descending`, `cmp_abs`, `sort_i32`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

The syntax looks scary at first. Read the `typedef` in the header like this: `binary_op_t` is "a pointer to a function that takes two `int32_t` and returns an `int32_t`". After that, a function pointer is used just like any other variable.

## Constraints

- `op_lookup` must search `OP_TABLE` in a loop (no `switch`).
- `sort_i32` compares values only through the comparator; no `qsort` (forbidden symbol).
- Comparators must not overflow (`return a - b;` does).
- cppcheck findings cost points.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- Also forbidden in this task: `qsort` (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings cost points (−3 each).

Files you may change (anything else makes the PR check fail):

- `tasks/18-function-pointers/src/ops.c`
- `tasks/18-function-pointers/ANSWERS.md`

## How to run

Inside `tasks/18-function-pointers/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-18`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
