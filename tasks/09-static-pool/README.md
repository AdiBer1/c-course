# Task 09: A static memory pool

Stage 3, Memory.

## What you build

A fixed-size block allocator over a static array: `pool_alloc` / `pool_free` with double-free and bad-pointer detection.

## Why this matters in embedded

Firmware that needs "dynamic" objects (message buffers, timers, network packets) but cannot risk heap fragmentation uses memory pools. FreeRTOS, Zephyr and lwIP all have them. Allocation time is bounded, the worst case is known at link time, and catching a double free immediately turns a mysterious corruption weeks later into an obvious error today.

## The API

Header `src/pool.h` (given):

- `void pool_init(void);`
- `void *pool_alloc(void);` returns a 32-byte block or `NULL`
- `bool pool_free(void *block);` rejects `NULL`, foreign pointers, mid-block pointers, double frees
- `size_t pool_free_count(void);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- No `malloc`/`calloc`/`realloc`/`free` (checked).
- All storage is `static` inside `pool.c`.
- Every block must be aligned for any type (`_Alignof(max_align_t)`).

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/09-static-pool/src/pool.c`
- `tasks/09-static-pool/ANSWERS.md`

## How to run

Inside `tasks/09-static-pool/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-09`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
