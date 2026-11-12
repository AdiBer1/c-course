# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
To run on a real board, you would only need to write a hardware-specific implementation of the HAL functions (`write` and `read`) that write to actual memory-mapped GPIO registers (such as STM32's `BSRR` or `ODR`). Zero code inside `led.c` would change, as `led.c` only operates through the abstract `hal_gpio_t` interface.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
`ctx` is a context pointer holding a reference to the instance-specific data (state) of a particular GPIO pin. Using `ctx` allows the HAL interface to remain reentrant and support multiple independent GPIO instances simultaneously. A global `fake_gpio_t` would cause state collisions when managing multiple pins or LEDs.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
Both are useful for testing logic without physical hardware: a mock records function calls to verify exact interaction sequences, while a fake mimics real state behavior. Mocking allows instantaneous, automated unit testing in milliseconds on a PC, capturing edge cases and race conditions without needing physical oscilloscopes or hardware setups.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
"Active low" means the LED turns ON when the GPIO pin outputs a low voltage (0 / logic LOW) and OFF at high voltage (1 / logic HIGH). The driver handles polarity inversion internally so higher-level application code can work with intuitive logical concepts (`led_set(true)`) without worrying about board wiring details.