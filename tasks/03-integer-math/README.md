# Task 03: Integer-only math with correct rounding

Stage 1, C basics.

## What you build

Temperature and ADC conversions using only integers, rounded to the nearest value, also for negative numbers.

## Why this matters in embedded

A Cortex-M0 or an AVR has no floating-point unit: one `float` division costs hundreds of cycles and pulls a software library into flash. Real firmware keeps temperatures in tenths of a degree and voltages in millivolts, and converts with integer math. A thermostat that always truncates is 0.1 °C off half the time; a battery monitor that overflows `int` on a 16-bit MCU shows nonsense.

## The API

Header `src/convert.h` (given):

- `int32_t c10_to_f10(int32_t c10);` tenths °C → tenths °F
- `int32_t f10_to_c10(int32_t f10);` tenths °F → tenths °C
- `uint16_t adc_to_mv(uint16_t raw);` 12-bit ADC reading → millivolts (3300 mV reference)

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- No `float`, no `double`, no `<math.h>`. (The defense will check.)
- Round to nearest, halves away from zero, including negative results.
- Intermediate results must not overflow on a chip where `int` is 16 bits: use `int32_t`/`uint32_t` explicitly.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/03-integer-math/src/convert.c`
- `tasks/03-integer-math/ANSWERS.md`

## How to run

Inside `tasks/03-integer-math/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-03`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
