# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
In C, integer division truncates. In 9 / 5 * c10, the division 9 / 5 is done first and gives 1, not 1.8, so the expression is just c10, which is wrong. For example, with c10 = 1000 it gives 1000 instead of 1800. In c10 * 9 / 5, the multiplication is done first, and only the final division truncates, so almost no precision is lost. So c10 * 9 / 5 is the right order: multiply first, divide last (and then round the final division).
## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
-41.8 comes, for example, from -209 / 5. C truncates toward zero, so -209 / 5 gives -41, but the nearest integer is -42. “Add half the divisor” works for positive numbers because adding moves the value up, away from zero, before truncation. For a negative number, adding moves the value toward zero: (-209 + 2) / 5 = -207 / 5 = -41, which is still wrong. So for a negative numerator I subtract half the divisor instead: (-209 - 2) / 5 = -211 / 5 = -42. In both cases the value is moved half a step away from zero, and then truncation toward zero gives the nearest integer.
## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
On an AVR, raw * 3300 would be computed in 16-bit int, whose maximum is 32767. Already raw = 10 gives 33000, and 4095 * 3300 is about 13.5 million, so the multiplication overflows. Because int is signed, this is undefined behaviour, and in practice the result would be a meaningless number. To make it safe, I copy raw into a uint32_t variable before the multiplication. Then the whole calculation is done in 32 bits on every chip, and the largest value, 13,513,500, fits easily in uint32_t.
## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Many small microcontrollers, such as a Cortex-M0 or an AVR, have no floating-point unit. Every float operation is then done in software by a library: one division can take hundreds of clock cycles instead of a few, and the library takes extra space in flash, which is very limited on such chips. It also costs more energy. With integers in small units (tenths of a degree, millivolts), the conversion is fast, small and exact enough, and the result is predictable, without floating-point rounding surprises.
