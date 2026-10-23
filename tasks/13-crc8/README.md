# Task 13: CRC-8

Stage 4, Bits & registers.

## What you build

The CRC-8/SMBus checksum, byte-wise and over a buffer, plus a check for frames that end with their CRC.

## Why this matters in embedded

Every byte that crosses a wire can be corrupted. I²C sensors (Sensirion SHT3x, for example) append a CRC-8 to each reading; SMBus, 1-Wire and many radio protocols do the same. The firmware recomputes it and throws away bad data instead of acting on a wrong temperature. Implementing it shows you that a "magic" algorithm is just shifts and XORs.

## The API

Header `src/crc8.h` (given; contains the algorithm description):

- `uint8_t crc8_update(uint8_t crc, uint8_t byte);`
- `uint8_t crc8(const uint8_t *data, size_t len);`
- `bool crc8_frame_ok(const uint8_t *frame, size_t len);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- Bit-by-bit algorithm (no lookup table needed).
- `-Wconversion` is on: `crc << 1` is an `int`, so cast explicitly.
- Handle `NULL` and short frames as documented.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/13-crc8/src/crc8.c`
- `tasks/13-crc8/ANSWERS.md`

## How to run

Inside `tasks/13-crc8/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-13`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
