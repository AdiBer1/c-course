# hal/: the mock hardware layer (stage 6)

Interfaces and simulators shared by tasks 19–21. Students read these files
but never change them.

| File | What it is |
|---|---|
| `hal_gpio.h` | GPIO interface: a struct of function pointers (`write`, `read`, `ctx`) |
| `mock_gpio.h/.c` | A test implementation of `hal_gpio_t` that records outputs and fakes inputs |
| `hal_clock.h` | Millisecond clock interface (`now_ms`, wraps at 2^32) |
| `mock_clock.h/.c` | A clock that only moves when the test advances it |
| `sim_uart.h/.c` | A simulated UART receiver with an RX interrupt and `hal_irq_disable/enable` |

A task links the files it needs through `HAL_SRCS` in its `task.mk`.

On a real microcontroller, `hal_gpio_t`, `hal_clock_t` and the UART/IRQ
functions would be implemented with the chip's registers. The task code that
uses them stays the same.
