# Task 15: Button debounce state machine

Stage 5, Embedded data structures.

## What you build

A tick-driven state machine that turns a bouncing button input into clean PRESS and RELEASE events.

## Why this matters in embedded

Every physical button bounces for a few milliseconds. Without debouncing, one press counts several times: a menu skips two entries, a counter jumps by three. The professional fix is a small state machine called from a periodic timer tick. It never blocks and handles any number of buttons. The same pattern (state, counter, tick) appears in protocol timeouts, LED blink patterns and watchdog logic.

## The API

Header `src/debounce.h` (given; describes the algorithm):

- `void debounce_init(debounce_t *d, uint8_t threshold_ticks);`
- `btn_event_t debounce_update(debounce_t *d, bool raw_pressed);`
- `btn_state_t debounce_state(const debounce_t *d);`
- `uint32_t debounce_press_count(const debounce_t *d);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- No delays, no waiting loops: one call = one tick of work.
- All state lives in the `debounce_t` (no `static` variables), so several buttons can be debounced independently.
- cppcheck findings cost points.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings cost points (−3 each).

Files you may change (anything else makes the PR check fail):

- `tasks/15-debounce/src/debounce.c`
- `tasks/15-debounce/ANSWERS.md`

## How to run

Inside `tasks/15-debounce/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-15`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
