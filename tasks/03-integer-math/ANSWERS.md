# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
In C integer division, the fractional part is truncated towards zero. Therefore, `9 / 5 * c10` performs the division first, resulting in `1` instead of `1.8`, which leads to severe precision loss before any multiplication. Conversely, `c10 * 9 / 5` multiplies first and divides last, preserving precision throughout the calculation.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
For positive numbers, adding half the divisor before division correctly rounds to the nearest integer. However, for negative numbers, simple addition causes the value to move closer to zero rather than away from it (halves away from zero), because C truncation always rounds towards zero. For negative results, subtracting half the divisor ensures proper rounding towards a greater absolute magnitude.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
On a microcontroller where `int` is 16 bits, the maximum representable value is 32,767. Since a maximum ADC reading of 4095 multiplied by 3300 equals 13,513,500, it vastly exceeds the 16-bit range and causes an integer overflow, producing completely incorrect results. To prevent this, the operand is explicitly cast to `uint32_t` prior to multiplication, ensuring safe calculation within a 32-bit range.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers (such as AVR or Cortex-M0) typically lack a hardware floating-point unit (FPU). Consequently, using `float` or `double` types requires software floating-point emulation libraries, which consume hundreds of precious clock cycles per operation and waste valuable flash memory. Integer arithmetic executes directly and much faster in hardware.
