# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
You would need to write a real board-specific HAL implementation for the microcontroller's GPIO registers. Nothing in led.c would change.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
ctx points to the specific hardware instance context, allowing multiple independent GPIO devices or fakes without global state collisions.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
The mock tests specific expected sequences and interactions, while the fake simulates actual stateful behavior. Testing on a mock allows running in milliseconds on a PC without hardware.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low means pulling the pin LOW turns the device on. The driver hides this complexity so application code only thinks in logical on/off terms.
