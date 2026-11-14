# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
I would need to write a real GPIO implementation for the specific hardware. It would connect the HAL functions to the actual hardware registers. The led.c code would not need to change because it already uses the HAL interface.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
ctx is a pointer to the specific GPIO object. It allows the same function to work with different GPIO instances. Using a global variable would make it difficult to manage multiple independent GPIO objects.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
The teacher's mock helps test the LED driver using controlled test conditions. My fake GPIO helps test and understand how the HAL interface works. Testing on a mock is repeatable and does not require physical hardware.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low means the LED is ON when the GPIO pin is LOW and OFF when it is HIGH. The driver handles this so the application can simply call led_set(true) to turn the LED on, regardless of the hardware configuration.

