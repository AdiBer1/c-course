# Task 07: Command-line parser

Stage 2, Arrays & pointers.

## What you build

Parse configuration lines like `LED=1` or `MOTOR_SPD=-250` into a struct, rejecting every malformed line with a precise error code.

## Why this matters in embedded

Devices are configured and debugged over serial consoles with exactly this kind of text protocol (think AT commands for a modem). The input comes from outside, so it can be anything: noise, a wrong baud rate, a malicious host. A parser that trusts its input overflows buffers; a parser that accepts "almost right" input applies wrong settings to real hardware.

## The API

Header `src/parser.h` (given): read the grammar and the **error order** there carefully.

- `parse_status_t parse_command(const char *line, command_t *out);`
- `const char *parse_status_str(parse_status_t status);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- No `atoi`, `strtol`, `sscanf` (forbidden symbols). Convert digits yourself.
- Detect an out-of-range value without ever overflowing `int32_t`.
- On error, `*out` must not change at all.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- Also forbidden in this task: `atoi`, `atol`, `atoll`, `strtol`, `strtoul`, `strtoll`, `sscanf` (−15 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/07-command-parser/src/parser.c`
- `tasks/07-command-parser/ANSWERS.md`

## How to run

Inside `tasks/07-command-parser/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-07`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
