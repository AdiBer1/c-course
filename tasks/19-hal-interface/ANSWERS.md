# Task 19 — Understanding questions

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:

I would need to write a real GPIO HAL implementation for the specific microcontroller. It would configure the GPIO pins and access the hardware registers in the `write` and `read` functions. The `led.c` file would not need to change because it already uses the HAL interface.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:

The `ctx` pointer stores a reference to the specific GPIO implementation instance. It allows the functions to access the correct fake GPIO structure. Using a global variable would make it harder to use multiple fake GPIO instances and would reduce flexibility.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:

The teacher's mock is useful for testing whether the LED driver sends the correct GPIO commands. My fake is useful because it also stores write history, counts reads and writes, and lets tests control input levels. Testing on a mock is fast, repeatable, and does not require physical hardware.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:

Active low means that a LOW signal activates the device instead of a HIGH signal. For example, an LED turns on when its GPIO pin is LOW. The driver handles this difference so the application code can simply call `led_set(led, true)` to turn the LED on, without knowing how the hardware is connected.