# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
C evaluates `*` and `/` left to right, and integer division throws away the remainder. In `9 / 5 * c10`, the first step is `9 / 5`, which is 1 in C, not 1.8, so the result is just `c10`: the conversion factor is already lost. In `c10 * 9 / 5` the multiplication happens first, so the full value reaches the division and only the final fractional part is lost, which we can then round. So `c10 * 9 / 5` is the right one: multiply before you divide.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
C division truncates toward zero, so -418 / 10 gives -41, but the nearest integer to -41.8 is -42. For a negative numerator I subtract half the divisor instead of adding it: (-418 - 5) / 10 = -423 / 10 = -42, which is correct. Adding half the divisor works for positive values because truncation moves them down and we pushed them up first. For negative values truncation moves them up, toward zero, so adding would push them even further in the wrong direction: (-418 + 5) / 10 = -413 / 10 = -41, still wrong. My `div_round` checks the sign of the numerator and pushes it half a divisor away from zero in the right direction.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
The largest product is 4095 * 3300 = 13,513,500, but a 16-bit `int` holds at most 32,767. Already at raw = 10 the product (33,000) does not fit. Signed overflow is undefined behaviour, so in practice the result would be garbage, and the voltage reading would jump around randomly. In my code I copy `raw` into a `uint32_t` before multiplying and use unsigned constants (`3300u`, `4095u`), so the whole calculation is done in 32 bits on every chip, no matter how wide `int` is.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Chips like the Cortex-M0 or AVR have no floating-point unit, so every `float` operation is done in software: one division can take hundreds of cycles, and the software float library takes a lot of flash memory, which is very limited on such chips. Integer math in small units (tenths of a degree, millivolts) gives the same precision we need, runs in a few cycles, and its rounding is exact and predictable.