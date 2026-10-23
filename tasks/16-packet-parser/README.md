# Task 16: Packet parser

Stage 5, Embedded data structures.

## What you build

A byte-by-byte state machine that receives framed packets (start byte, length, payload, checksum), plus a frame builder.

## Why this matters in embedded

Serial links deliver a stream of bytes with no boundaries. Every binary protocol (SLIP, HDLC, Modbus RTU, MAVLink on drones, the link between a flight controller and an ESC) needs a parser exactly like this one: fed one byte at a time from an interrupt or a poll loop, robust against garbage and lost bytes, and never trusting a length field from the wire.

## The API

Header `src/packet.h` (given; contains the frame format and state diagram):

- `void pkt_init(pkt_parser_t *p);`
- `pkt_result_t pkt_feed(pkt_parser_t *p, uint8_t byte);`
- `const uint8_t *pkt_payload(const pkt_parser_t *p, uint8_t *len_out);`
- `uint32_t pkt_error_count(const pkt_parser_t *p);`
- `size_t pkt_build(uint8_t *out, size_t out_size, const uint8_t *payload, uint8_t len);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- One byte per call; never look ahead or back.
- Validate LEN before storing any payload byte.
- A failed frame must not destroy the last good payload.
- cppcheck findings cost points.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings cost points (−3 each).

Files you may change (anything else makes the PR check fail):

- `tasks/16-packet-parser/src/packet.c`
- `tasks/16-packet-parser/ANSWERS.md`

## How to run

Inside `tasks/16-packet-parser/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-16`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
