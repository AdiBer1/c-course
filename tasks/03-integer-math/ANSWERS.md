# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
The first version is right because in C, both operators have the same precedence and run left to right using integer division, which cuts off the decimal part. 9 / 5 runs first and turns 1.8 into 1, losing accuracy before multiplying by c10.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
In C, integer division always cuts off toward zero, so adding half the divisor pushes a negative number closer to zero instead of away from it. To round negative numbers away from zero, we check if the numerator is negative and add half the divisor instead of adding it.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
On a 16-bit int system, the max value is 32,767, so raw * 3300 would overflow and produce wrong numbers. To make it safe, I cast raw to uint32_t before multiplying so the calculation happens in 32-bit math for best accuracy.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers usually do not have a hardware floating-point unit. Using float forces the compiler to include a heavy software math library.
