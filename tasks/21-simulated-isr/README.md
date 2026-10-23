# Task 21: A simulated interrupt

Stage 6, Separating logic from hardware.

## What you build

An interrupt handler that receives UART bytes into a ring buffer and signals complete lines through a `volatile` flag, and a main-loop function that takes lines out safely.

## Why this matters in embedded

Interrupts are how a microcontroller reacts to the outside world in microseconds: a byte arrives, a timer expires, a pin changes. The ISR must be tiny; the real work happens in the main loop; and every variable they share is a potential race condition. `volatile` and critical sections are where many embedded bugs hide, the kind that appear once a week under load. **Code written this way against the HAL moves to a real UART almost unchanged:** only `hal_uart_read_dr` and the interrupt enable/disable become real register accesses.

## The API

Simulator `hal/sim_uart.h` (shared, given): `hal_uart_read_dr`, `hal_irq_disable`, `hal_irq_enable`, `hal_uart_attach_isr`. The tests play the outside world with `sim_uart_receive`.

Header `src/uart_rx.h` (given):

- `void uart_rx_init(void);`
- `void uart_rx_isr(void);`
- `bool uart_rx_line_ready(void);`
- `bool uart_rx_read_line(char *dst, size_t dst_size);`
- `size_t uart_rx_available(void);`
- `uint32_t uart_rx_dropped(void);`

The documentation in the header is the specification: read it fully before you start. Headers marked "Do not change this file" are not yours to edit.

## Constraints

- The ISR always reads the data register, stays short, and never waits.
- Shared variables are `volatile`.
- The main-loop side protects read-modify-write of shared state with `hal_irq_disable()` / `hal_irq_enable()`, and always re-enables.
- No `malloc`; all state static in `uart_rx.c`.
- cppcheck findings cost points.

Checked automatically for this task:

- Compiler warnings cost points (−5 each).
- `malloc`, `calloc`, `realloc`, `free` are forbidden (−15 each).
- `-Wconversion` is on: implicit narrowing conversions are warnings.
- cppcheck findings cost points (−3 each).

Files you may change (anything else makes the PR check fail):

- `tasks/21-simulated-isr/src/uart_rx.c`
- `tasks/21-simulated-isr/ANSWERS.md`

## How to run

Inside `tasks/21-simulated-isr/`:

```bash
make          # build your code and the tests
make test     # build and run the tests (sanitizer reports appear here)
make score    # compute your score exactly like CI does (also: build/score.md)
make clean    # remove build/
```

You need **60 or more** in `make score` before you open the PR. Branch name: `task-21`.

## Understanding questions

Answer the questions in [`ANSWERS.md`](ANSWERS.md), in your own words. An empty `ANSWERS.md` costs 10 points, and the oral defense will start from your answers.
