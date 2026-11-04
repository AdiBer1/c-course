# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
Integer division in C truncates the remainder toward zero. Evaluating 9 / 5 first results in 1 (instead of 1.8), which causes a severe loss of precision right at the start. Writing c10 * 9 / 5 multiplies first and performs the integer division last, preserving accuracy.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
Adding half the divisor pushes positive values toward the correct rounded integer. However, for negative numbers, because C truncates toward zero, adding half would shift the value in the wrong direction (closer to zero). To round correctly, we must check the sign and subtract half the divisor for negative results.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
The maximum raw value of 4095 multiplied by 3300 equals 13,513,500, which exceeds the maximum limit of a 16-bit integer (~32,767), causing an overflow. We made the code safe by casting raw to a 32-bit unsigned integer (uint32_t) before performing the multiplication.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers often lack a hardware Floating-Point Unit (FPU). Using float forces software emulation, which is extremely slow and consumes precious CPU cycles and flash memory, whereas integer math is fast and highly efficient for hardware.

