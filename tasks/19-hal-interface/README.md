# Task 19: A HAL interface and a mock

Stage 6, Separating logic from hardware.

## What you build

LED and button drivers that only talk to a GPIO interface (a struct of function pointers), plus your own fake GPIO that implements that interface.

## Why this matters in embedded

This is how professional firmware is structured: drivers and application logic are written against a Hardware Abstraction Layer, never against registers. On the PC, a mock implements the HAL and the logic is tested in milliseconds. On the board, a real HAL implementation is plugged in. **Code written against a HAL moves to real hardware almost unchanged**: only the small HAL implementation is chip-specific.

## The API

Interface `hal/hal_gpio.h` (shared, given): `hal_gpio_t` with `write`, `read` and `ctx`.

Header `src/led.h` (given): `led_init`, `led_set`, `led_toggle`, `led_is_on`, `button_init`, `button_is_pressed`.

Header `src/fake_gpio.h` (given): `fake_gpio_init`. You implement the fake's `write`/`read` functions in `src/fake_gpio.c`.

The tests use the teacher's mock (`hal/mock_gpio.h`) **and** your fake.

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- `led.c` uses only `gpio->write` / `gpio->read`; no globals.
- `fake_gpio.c` keeps all state in the `fake_gpio_t` reached through `ctx`.
- cppcheck findings cost points.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings cost points (−3 each).

Files you may change (anything else makes the PR check fail):

- `tasks/19-hal-interface/src/led.c`
- `tasks/19-hal-interface/src/fake_gpio.c`
- `tasks/19-hal-interface/ANSWERS.md`

## How to run

Inside `tasks/19-hal-interface/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-19`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
