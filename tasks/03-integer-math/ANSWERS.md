# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
In C, dividing two integers truncates. In `9 / 5 * c10` the division is done
first, so `9 / 5` becomes 1 and the result is just `c10`. The 0.8 part is
lost completely. In `c10 * 9 / 5` the multiplication is done first, so the
number is large before we divide, and we only lose less than one unit at the
end. So `c10 * 9 / 5` is the right one: with integer math you multiply first
and divide last (as long as the multiplication does not overflow).

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
C integer division truncates toward zero. For a positive number, toward zero
means down, so adding half the divisor first turns truncation into rounding.
For a negative number, toward zero means up, so adding half pushes it the
wrong way. Example: -418 / 10 should give -42. With the "add half" trick:
(-418 + 5) / 10 = -413 / 10 = -41, which is wrong. For negative numbers I
subtract half the divisor instead: (-418 - 5) / 10 = -423 / 10 = -42, which
is correct. My helper `div_round` checks the sign and adds or subtracts half
accordingly, so halves are always rounded away from zero.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
A 16-bit `int` can hold at most 32767. Already for raw = 10, 10 * 3300 = 33000
is too big, and for raw = 4095 the product is 13,513,500. That is signed
overflow, which is undefined behaviour, so the result would be garbage (for
example a negative or a random-looking voltage). To prevent this I cast `raw`
to `uint32_t` before multiplying: `(uint32_t)raw * 3300U`. Now the whole
calculation is done in 32 bits on every chip, and the largest value
(about 13.5 million) fits easily.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small microcontrollers like an AVR or a Cortex-M0 have no floating-point unit
(FPU). Every float operation must be done in software by a library function,
which takes hundreds of cycles and adds a lot of code to the limited flash
memory. Integer math is fast, small, and has predictable timing. By keeping
values in small integer units (tenths of a degree, millivolts) we get the
precision we need without using float at all.