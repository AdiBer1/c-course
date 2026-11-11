# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer: Multiplying two 32-bit values can produce a result that does not fit in 32 bits, so int64_t is needed to prevent intermediate overflow. Each Q16.16 value is scaled by 65536, so their product is scaled by 65536 × 65536. We divide by 65536 once to bring the result back to the Q16.16 scale.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer: Left-shifting a negative signed integer can cause undefined behavior in C. Large values can also overflow the 32-bit result. Multiplying by 65536 using an int64_t intermediate is safer because it allows us to detect values outside the Q16.16 range and saturate them.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer: With wrapping arithmetic, the integral can overflow and suddenly change from a large positive value to a negative value, or vice versa. This can cause the PID controller to calculate an incorrect output and make the motor behave unpredictably. Saturation keeps the value within its allowed range and prevents this sudden wraparound. A real PID controller may also need anti-windup logic.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer: Q16.16 is a reasonable choice for battery voltage because its range easily covers 0–60 V and its resolution is sufficient for many voltage measurements. However, it is not suitable for GPS coordinates because its resolution is too coarse to represent changes of 0.0000001 degrees accurately. A wider fixed-point format or a scaled integer representation, such as storing degrees multiplied by 10,000,000, would be more suitable.

