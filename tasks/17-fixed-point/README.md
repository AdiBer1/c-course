# Task 17: Q16.16 fixed-point arithmetic

Stage 5, Embedded data structures.

## What you build

Fractional arithmetic without floating point: conversions, saturating add/sub, and rounded, saturating multiply and divide in Q16.16.

## Why this matters in embedded

PID motor controllers, digital filters and sensor calibration curves need fractions, but many MCUs have no FPU, and even with one some code must be bit-exact and deterministic. Q formats give fractional math with plain integer instructions. Getting the intermediate width, the rounding or the saturation wrong makes a motor jerk or a filter drift.

## The API

Header `src/fixed.h` (given):

- `q16_from_int`, `q16_to_int`, `q16_from_milli`, `q16_to_milli`
- `q16_add`, `q16_sub` (saturating)
- `q16_mul`, `q16_div` (rounded, saturating; divide-by-zero rules in the header)

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- No `float`/`double` (extra float warnings are enabled for this task).
- Round to nearest, halves away from zero.
- Saturate instead of overflowing; never overflow an intermediate (use `int64_t`).
- No left shifts of negative numbers (undefined): multiply instead.
- cppcheck findings cost points.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings cost points (−3 each).

Files you may change (anything else makes the PR check fail):

- `tasks/17-fixed-point/src/fixed.c`
- `tasks/17-fixed-point/ANSWERS.md`

## How to run

Inside `tasks/17-fixed-point/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-17`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
