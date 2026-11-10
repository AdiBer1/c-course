# Task 03 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `9 / 5 * c10` and `c10 * 9 / 5` look the same in math. Why do they give different results in C, and which one is right?

Answer:
Integer division in C truncates the remainder, and operators of equal precedence are evaluated left to right. In `9 / 5 * c10`, the division `9 / 5` runs first and gives 1, so the 0.8 is lost and the result is just `c10`. In `c10 * 9 / 5`, the multiplication runs first and the truncation happens only once, on the final result. So `c10 * 9 / 5` is the right one, because it keeps full precision until the last division.

## Q2. Explain how you round to the nearest integer for a negative result such as -41.8. Why does the trick "add half the divisor" alone fail for negative numbers?

Answer:
C integer division truncates toward zero. For non-negative numerators, adding `d/2` before dividing pushes any remainder of one half or more up to the next integer, which is correct rounding. For negative numerators the same addition moves the value toward zero, which is the wrong direction: for example `(-418 + 5) / 10` gives -41 instead of -42. So for negative numerators I subtract `d/2` instead, which rounds halves away from zero as the task requires.

## Q3. On an 8-bit AVR, `int` is 16 bits. What would happen in `adc_to_mv` if the multiplication `raw * 3300` were done in `int` there? How did you make your code safe on such a chip?

Answer:
The largest product is 4095 * 3300 = 13,513,500, but a 16-bit `int` holds at most 32,767 (and a 16-bit unsigned value at most 65,535). The multiplication would overflow: for signed `int` this is undefined behaviour, and in any case the result would be wrong. I cast `raw` to `uint32_t` before multiplying, so the whole calculation is done in 32 bits and cannot overflow.

## Q4. Why would firmware on a small microcontroller avoid `float` for this kind of conversion?

Answer:
Small chips such as a Cortex-M0 or an AVR have no floating-point unit, so every `float` operation is emulated in software. It costs hundreds of cycles per operation and pulls a software floating-point library into flash. Integer math is faster, smaller and predictable, and with units like tenths of a degree or millivolts it is accurate enough for the job.