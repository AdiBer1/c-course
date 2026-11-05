# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
In C, integer division removes the fractional part. In `9 / 5 * c10`, the division happens first and `9 / 5` becomes `1`. In `c10 * 9 / 5`, the multiplication happens first, so less precision is lost. The second expression is the better choice for this conversion, although rounding may still be needed.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
To round -41.8 to the nearest integer, the result should be -42. Rounding must take the sign into account: for negative values, we work with the magnitude and then restore the negative sign. Simply adding half the divisor does not work because it moves a negative value toward zero instead of rounding it correctly.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
On a 16-bit int system, the multiplication `raw * 3300` can overflow because the result can be larger than the maximum value of a 16-bit int. I made the calculation safe by converting `raw` to `uint32_t` before multiplying, so the multiplication is performed using a 32-bit unsigned integer.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers often do not have a floating-point unit. Floating-point calculations can therefore take more time and require extra software libraries, which use more memory. Integer calculations are usually faster and more suitable for this kind of embedded conversion.
