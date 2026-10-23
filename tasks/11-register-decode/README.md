# Task 11: Decoding a status register

Stage 4, Bits & registers.

## What you build

Decode a 32-bit status register of a fake sensor chip into a struct (flags, mode, FIFO level, signed temperature, version) and encode it back.

## Why this matters in embedded

Reading a datasheet register table and turning it into code is daily firmware work. Every sensor, radio and motor driver reports its state this way. Getting the sign extension of a 12-bit temperature wrong means -1 °C reads as +255.9 °C; reading the register twice means combining two different moments; forgetting `volatile` lets the compiler skip the read entirely.

## The API

Header `src/status_reg.h` (given): contains the register layout table.

- `status_t status_decode(uint32_t reg);`
- `uint32_t status_encode(const status_t *s);`
- `status_t status_read(const volatile uint32_t *reg);` read exactly once
- `int16_t status_temp_celsius(const status_t *s);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- `-Wconversion` is on: make every narrowing explicit.
- Read the register through the pointer exactly once in `status_read`.
- No magic numbers scattered around: define the field positions and masks once.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/11-register-decode/src/status_reg.c`
- `tasks/11-register-decode/ANSWERS.md`

## How to run

Inside `tasks/11-register-decode/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-11`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
