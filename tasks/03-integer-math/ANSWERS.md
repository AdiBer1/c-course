# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
In C, operations evaluate from left to right. `9 / 5 * c10` performs integer division `9 / 5` first, truncating to `1` and leaving `1 * c10`, which loses all fractional precision. `c10 * 9 / 5` multiplies first, preserving precision before dividing, which makes it the correct expression.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
C integer division truncates toward zero. Adding half the divisor pushes positive numbers away from zero for correct rounding, but for negative values, adding a positive offset shifts the number *towards* zero, rounding in the wrong direction. For negative results, we subtract half the divisor (or adjust offset based on sign) to push the dividend away from zero before dividing.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
On a system where `int` is 16 bits, `raw * 3300` can reach $4095 \times 3300 = 13,513,500$, overflowing the signed 16-bit maximum (32,767) and causing undefined behavior or wrapped results. The code is made safe by explicitly casting to `uint32_t` or `int32_t` so the arithmetic is guaranteed to evaluate in 32-bit width.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers often lack a hardware Floating-Point Unit (FPU). Using `float` forces the compiler to pull large software emulation libraries into flash memory and uses significantly more CPU cycles compared to fast, fixed-width integer arithmetic.

