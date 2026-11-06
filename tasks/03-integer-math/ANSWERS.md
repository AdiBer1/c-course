# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
Answer:9 / 5 * c10 performs integer division first (9 / 5), which truncates to 1 and completely destroys precision. c10 * 9 / 5 multiplies first to maintain maximum precision before dividing, which is the correct approach.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
For positive numbers, adding half the divisor shifts values up to round correctly. For negative numbers (e.g. -41.8), adding half the divisor moves it toward zero (making it -37.3 truncated to -37), which rounds in the wrong direction. We must subtract half the divisor instead for negative numbers to move it away from zero.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
On a 16-bit int chip, raw * 3300 can exceed 32,767 (the signed 16-bit max), causing signed integer overflow and garbage values. I made the code safe by explicitly casting raw to uint32_t so the calculation occurs in a safe 32-bit container.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers often lack a hardware Floating-Point Unit (FPU). Using float pulls in heavy emulation libraries, dramatically increasing code size, execution time, and power consumption.
