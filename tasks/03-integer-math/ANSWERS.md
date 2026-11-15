# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
The second one is better - because is C what will happen is - that first it will calculate 9/5 - this is almost 2, but these are 2 ints, so it rounds it to an int - 1, then it will multiply it - the one, by c10, the result will be c10, instead of c10*1.8!!!
## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
Rounding determines the closest integer. The trick of adding 0.5 (or half the divisor) succeeds for positive numbers because standard integer truncation drops the fractional part, automatically rounding up if the original fraction was 0.5 or greater.

However, for negative numbers, adding 0.5 moves the value closer to zero. For example, if we add 0.5 to -41.8, we obtain -41.3. Truncating this yields -41, which is incorrect because -41.8 is closer to -42.

Because negative numbers operate as a mirror image of positive numbers across zero, our arithmetic must also mirror the operation. To correctly round negative numbers, we must subtract 0.5 (or subtract half the divisor) before applying the truncation.
## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
On an 8-bit AVR where an int is 16 bits, the multiplication raw * 3300 would cause an overflow if the result exceeds the maximum value a 16-bit integer can hold. When an overflow occurs, the value wraps around, a value slightly exceeding the maximum will wrap around to become a very small number in the case of unsigned. Because we need to allow for values at the upper limit and ensure they are not incorrectly interpreted as a very low voltage due to this wraparound, we must prevent the overflow. To make the code safe on such a chip, you cast the variables (like raw or 3300) to a larger data type, such as a 32-bit long , before performing the multiplication. This ensures the calculation has enough capacity to hold the result without wrapping around.
## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Because most of these chips don't have a hardware Floating-Point Unit (FPU). Without an FPU, the chip has to simulate float calculations in software, which uses a lot more CPU clock cycles and really slows down the processor. On top of that, adding the required software libraries just to handle those calculations eats up a big chunk of our limited flash memory.
