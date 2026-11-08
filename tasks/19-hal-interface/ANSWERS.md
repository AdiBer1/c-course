# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
write a real GPIO HAL that controls the hardware registers. The led.c code would stay the same because it only uses the HAL functions.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
ctx points to the specific fake GPIO instance. It allows each fake to keep its own data without using global variables.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
checks how the driver uses the HAL, while the fake lets to test the GPIO implementation itself. Testing on a mock is faster and safer than testing on real hardware.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low means that LOW turns the LED on, and HIGH turns it off. The driver handles this so the rest of the code can simply use ON and OFF.

