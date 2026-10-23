# Task 04: Enums and switch

Stage 1, C basics.

## What you build

Classify single-character console commands into an `enum`, compute the next LED state, and give each command a printable name.

## Why this matters in embedded

Small devices are often controlled with one-letter commands over a serial line ("1" = on, "s" = status). Firmware turns the character into an enum as early as possible, then the rest of the code works with named values instead of magic characters, and the compiler warns when a `switch` forgets a case. Defensive `default:` branches keep the device safe when memory corruption or a bug produces an impossible value.

## The API

Header `src/command.h` (given):

- `command_t command_classify(char c);`
- `led_state_t led_next_state(led_state_t current, command_t cmd);`
- `const char *command_name(command_t cmd);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- Use `switch` statements.
- Handle out-of-range enum values (e.g. 42) safely, as described in the header.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- cppcheck findings are shown as notes only (no points lost before stage 5).

Files you may change (anything else makes the PR check fail):

- `tasks/04-enums-and-switch/src/command.c`
- `tasks/04-enums-and-switch/ANSWERS.md`

## How to run

Inside `tasks/04-enums-and-switch/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-04`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
