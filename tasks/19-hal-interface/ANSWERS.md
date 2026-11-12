# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
To run on a real board, you would only write a small board-specific HAL implementation (e.g., `stm32_gpio.c`) whose `write` and `read` functions manipulate the microcontroller's GPIO registers (`ODR`/`BSRR` and `IDR`), and populate a `hal_gpio_t` struct with those function pointers. Nothing in `led.c` needs to change at all, because it interacts exclusively through the `hal_gpio_t` interface.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
The `ctx` pointer passes instance-specific state to the HAL callbacks so the same `fake_write` and `fake_read` functions can operate on whichever `fake_gpio_t` struct is bound to that `hal_gpio_t`. Using a global `fake_gpio_t` would prevent running multiple independent fake GPIO instances simultaneously (for example, simulating two separate GPIO expanders or running isolated tests without cross-contamination).

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
Testing against two independent HAL implementations verifies that the driver strictly adheres to the `hal_gpio_t` contract without relying on quirks of one specific fake. Running on a mock on a PC executes in milliseconds without flashing hardware, lets you deterministically inject exact input sequences or edge cases, and automatically asserts the exact order and count of every pin write and read.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Electrically, "active low" means the load (such as an LED connected to VCC through a resistor) turns on when the MCU pin sinks current by driving `GPIO_LOW` (0 V), and turns off when driven `GPIO_HIGH` (VCC). Handling polarity inversion inside the driver encapsulates the board wiring details so application code only deals with clean logical concepts (`on`/`off` and `pressed`), allowing the hardware wiring to change without modifying application logic.