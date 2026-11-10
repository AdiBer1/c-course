# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
The two expressions give different results because C does integer division. In the first expression, 9 / 5 gives 1, so some precision is lost. In the second expression, c10 is multiplied by 9 before dividing by 5, so the result is more accurate.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
For a negative number, I check the remainder and decide if I need to round down. For example, -41.8 becomes -42. Adding half the divisor does not work by itself for negative numbers because negative values need different handling.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
On an AVR, int is only 16 bits, so the result of raw * 3300 can be too large. This can cause an overflow and give a wrong result. I used uint32_t before the multiplication so there is enough space for the result.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers have limited memory and processing power. Float calculations can take more time and resources. Integer calculations are usually simpler and faster for this kind of conversion.
