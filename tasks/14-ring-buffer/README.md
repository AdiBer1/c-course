# Task 14: Ring buffer

Stage 5, Embedded data structures.

## What you build

A FIFO byte queue over caller-provided storage, with wrap-around indices, full/empty detection and bulk read/write.

## Why this matters in embedded

The ring buffer is the most used data structure in firmware: UART drivers, audio streams, log queues, USB endpoints. An interrupt pushes, the main loop pops, and the storage never moves or grows. Off-by-one errors in wrap-around and full/empty detection are classic bugs that lose or duplicate data only under load.

## The API

Header `src/ring_buffer.h` (given):

- `rb_init`, `rb_push`, `rb_pop`, `rb_peek`
- `rb_count`, `rb_free`, `rb_is_empty`, `rb_is_full`, `rb_clear`
- `rb_write`, `rb_read` (bulk)

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- No `malloc`; the caller provides the storage.
- All `size` bytes must be usable.
- O(1) per byte.
- From this stage on, **cppcheck findings cost points** (−3 each).

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings cost points (−3 each).

Files you may change (anything else makes the PR check fail):

- `tasks/14-ring-buffer/src/ring_buffer.c`
- `tasks/14-ring-buffer/ANSWERS.md`

## How to run

Inside `tasks/14-ring-buffer/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-14`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
