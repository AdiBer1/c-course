# Task 02: Fixed-width types and saturation

Stage 1, C basics.

## What you build

Saturating add and subtract for `uint8_t` and `int16_t`: results that do not fit are clamped instead of wrapping.

## Why this matters in embedded

A PWM duty cycle, an audio sample or a sensor offset that overflows by one step should stay at its maximum. Wrapping turns it into its minimum instead: full brightness suddenly goes dark, a loud sample becomes a click. DSP instructions on ARM Cortex-M4 (`QADD`, `UQADD8`) exist for exactly this. And `stdint.h` types are the only way to say "exactly 8 bits" on every compiler.

## The API

Header `src/saturate.h` (given):

- `uint8_t sat_add_u8(uint8_t a, uint8_t b);`
- `uint8_t sat_sub_u8(uint8_t a, uint8_t b);`
- `int16_t sat_add_i16(int16_t a, int16_t b);`
- `int16_t sat_sub_i16(int16_t a, int16_t b);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- No undefined behaviour: signed overflow is never allowed, not even in an intermediate step (UBSan checks it).
- Use the `stdint.h` limit macros (`UINT8_MAX`, `INT16_MIN`, ...) instead of magic numbers.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/02-fixed-width-types/src/saturate.c`
- `tasks/02-fixed-width-types/ANSWERS.md`

## How to run

Inside `tasks/02-fixed-width-types/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-02`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
