# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Multiplying two 32-bit `q16_t` numbers can produce a 64-bit value, so an `int64_t` intermediate is required to prevent 32-bit signed overflow before scaling back. Since both `a` and `b` are scaled by $2^{16}$ (65536), their product `a * b` is scaled by $65536 \times 65536$; dividing by 65536 once removes one scaling factor so the final result remains in Q16.16 format.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
In C, left-shifting a negative signed integer is undefined behavior, whereas multiplying by 65536 in `int64_t` is completely well-defined. Additionally, for large values outside `-32768..32767`, `x << 16` overflows a 32-bit integer, whereas computing in `int64_t` allows us to detect out-of-range values and saturate cleanly to `Q16_MAX` or `Q16_MIN`.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping arithmetic, exceeding `Q16_MAX` causes the integral term to abruptly wrap around to a large negative value (`Q16_MIN`), suddenly commanding the motor to reverse at full power and causing violent jerks or hardware damage. Saturation clamps the accumulator at the maximum limit so the motor continues applying smooth, bounded full effort safely.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
For a 0–60 V battery voltage, Q16.16 is a great choice because 0–60 fits easily within ±32768 and the 0.000015 V resolution is far finer than typical ADC measurements require. For GPS coordinates (±180° needing 0.0000001° resolution), Q16.16 is a poor choice because its step size (~0.000015°) is 150 times too coarse, introducing over a meter of position error; a format with more fractional bits (such as Q9.23) would be needed.