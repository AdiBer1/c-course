# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
In C, integer division removes the decimal part.
So `9 / 5` becomes `1` before the multiplication.
In `c10 * 9 / 5`, we multiply first and divide after.
The second one is correct, but we still need rounding to get the nearest integer.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
For negative numbers, I subtract half the divisor instead of adding it.
For example, -41.8 should become -42.
Adding half the divisor does not work because C division truncates toward zero, so it can give the wrong result.


## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
The multiplication can overflow because the result can be much bigger than the maximum value of a 16-bit int.
I used `uint32_t` before multiplying, so the calculation uses 32 bits and can safely hold the result.


## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Many small microcontrollers do not have a floating-point unit.
Float calculations can be slower and use more memory.
Integer math is faster and more efficient for these conversions.
