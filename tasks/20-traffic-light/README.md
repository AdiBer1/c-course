# Task 20: Non-blocking traffic light

Stage 6, Separating logic from hardware.

## What you build

A traffic-light state machine with a pedestrian button, driven by a mock clock and a mock GPIO, that never blocks and survives clock wrap-around.

## Why this matters in embedded

Real firmware does many things at once on one CPU without an operating system: blink a status LED, read buttons, talk over UART, control a motor. The only way is non-blocking code: each task checks the time, does a little work and returns. One `delay()` in one module breaks everything else. The wrap-safe time comparison taught here is required in every project that uses a millisecond tick. **Because the logic only uses the HAL, it moves to real hardware almost unchanged.**

## The API

Interfaces `hal/hal_gpio.h` and `hal/hal_clock.h` (shared, given).

Header `src/traffic_light.h` (given; contains the timing and pedestrian rules):

- `bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock);`
- `void tl_update(traffic_light_t *tl);`
- `tl_phase_t tl_phase(const traffic_light_t *tl);`
- `bool tl_pedestrian_waiting(const traffic_light_t *tl);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- Never wait: no loops on the clock, no `sleep` (forbidden symbols).
- Compare times only as `(uint32_t)(now - start) >= duration`.
- All hardware access through the HAL structs.
- cppcheck findings cost points.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- Also forbidden in this task: `sleep`, `usleep`, `nanosleep` (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings cost points (−3 each).

Files you may change (anything else makes the PR check fail):

- `tasks/20-traffic-light/src/traffic_light.c`
- `tasks/20-traffic-light/ANSWERS.md`

## How to run

Inside `tasks/20-traffic-light/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-20`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
