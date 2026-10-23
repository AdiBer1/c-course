# Task 10: Bit manipulation macros

Stage 4, Bits & registers.

## What you build

Macros to set, clear, toggle and read single bits and to extract and insert bit fields, plus three small bit functions.

## Why this matters in embedded

Every peripheral on a microcontroller is controlled by setting bits in registers: "set bit 5 of GPIOA->ODR to turn on the LED", "write 0b101 into bits 4..6 of the clock register". Vendor headers are full of exactly these macros. Getting one wrong (a signed shift, a missing parenthesis, a mask that clears the wrong bits) changes a different peripheral's configuration and is very hard to debug.

## The API

**This task is different:** you write the macros themselves in `src/bit_macros.h` (a student file), following the rules written at the top of that file:

- `BIT(n)`, `BIT_SET(reg, n)`, `BIT_CLEAR(reg, n)`, `BIT_TOGGLE(reg, n)`, `BIT_READ(reg, n)`
- `FIELD_MASK(pos, width)`, `FIELD_GET(reg, pos, width)`, `FIELD_SET(reg, pos, width, val)`

and in `src/bits.c` (header `src/bits.h`, given):

- `unsigned bits_count_ones(uint32_t x);`
- `bool bits_is_power_of_two(uint32_t x);`
- `uint8_t bits_swap_nibbles(uint8_t x);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- Macros are expressions yielding `uint32_t`; they never modify their arguments.
- Parenthesise every macro parameter and the whole macro.
- No undefined shifts (shifting a 32-bit value by 32, shifting a 1 into the sign bit of an `int`).
- `-Wconversion` is on from this stage: every narrowing must be an explicit cast.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/10-bit-macros/src/bit_macros.h`
- `tasks/10-bit-macros/src/bits.c`
- `tasks/10-bit-macros/ANSWERS.md`

## How to run

Inside `tasks/10-bit-macros/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-10`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
