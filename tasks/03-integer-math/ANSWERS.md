## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
In C, integer division truncates towards zero. Writing `9 / 5 * c10` performs `9 / 5` first, resulting in `1`, which then multiplies by `c10` and loses all fractional precision. Writing `c10 * 9 / 5` multiplies first to preserve precision before dividing, which is the correct approach.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
Adding half the divisor shifts numbers toward zero for negative values instead of rounding away from zero. For negative numbers, we must subtract half the divisor (e.g., `-5` for division by 10) before truncating, ensuring correct rounding of halves away from zero.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
Since a 16-bit signed `int` maxes out at 32,767, multiplying `raw` (up to 4095) by 3300 yields over 13 million, causing a severe integer overflow and undefined behavior. I made it safe by explicitly casting `raw` to `uint32_t` before the multiplication.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers lack a hardware Floating-Point Unit (FPU), meaning `float` operations are emulated in software, taking hundreds of slow clock cycles and bloating the flash memory with large runtime libraries.