# Task 12: Endianness

Stage 4, Bits & registers.

## What you build

Read and write 16- and 32-bit values in little- and big-endian byte order from a byte buffer, and detect the host's own order.

## Why this matters in embedded

A Modbus register, a CAN frame, a BMP280 pressure reading, a TCP header: each specifies its own byte order, and it is often not the CPU's. Code that casts a byte buffer to `uint32_t *` works on one chip and returns garbage (or a HardFault from an unaligned access) on the next. Byte-by-byte conversion is portable, alignment-safe and clear.

## The API

Header `src/byte_order.h` (given):

- `rd_u16_le`, `rd_u16_be`, `rd_u32_le`, `rd_u32_be`
- `wr_u16_le`, `wr_u16_be`, `wr_u32_le`, `wr_u32_be`
- `int16_t rd_i16_be(const uint8_t *p);`
- `bool host_is_little_endian(void);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- Build values with shifts and ORs only.
- No `memcpy`, no `htonl`/`ntohl` (forbidden symbols), no casting the buffer to a wider pointer type.
- The buffer may be unaligned.
- No undefined shifts: `uint8_t` is promoted to signed `int` before shifting.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- Also forbidden in this task: `memcpy`, `memmove`, `htons`, `htonl`, `ntohs`, `ntohl` (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/12-endianness/src/byte_order.c`
- `tasks/12-endianness/ANSWERS.md`

## How to run

Inside `tasks/12-endianness/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-12`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
