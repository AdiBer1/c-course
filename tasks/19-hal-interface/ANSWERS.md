# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
On a real board I would need to implement a GPIO HAL for that specific microcontroller. It would provide the `write` and `read` functions and connect them to the real GPIO registers. The `led.c` code would not need to change, because it already works through the `hal_gpio_t` interface.


## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
`ctx` lets the function pointer know which GPIO object it should work with. This makes it possible to have more than one fake GPIO at the same time. Using a global `fake_gpio_t` would make the fake depend on one global instance and would make it harder to reuse and test.


## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
The teacher's mock lets the tests check how the driver uses the HAL, such as how many writes are made and which values are written. My fake gives me another implementation of the same interface that I control. Testing on a mock is also much faster and more predictable than testing on real hardware, and it can check exact behavior without needing a physical board.


## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low means that the device is activated when the GPIO is LOW instead of HIGH. For an active-low LED, LOW means on and HIGH means off. The driver handles this so the rest of the program can simply use `led_set(led, true)` to turn the LED on without knowing how the hardware is electrically connected.
