# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Multiplying two 32-bit integers can produce up to a 64-bit product, so `int64_t` prevents intermediate signed overflow before rescaling. Since both `a` and `b` are scaled by $2^{16}$ (65536), their raw product `(A * 65536) * (B * 65536)` has a scaling factor of $65536^2$; dividing by 65536 once leaves one factor of 65536 so the result remains in Q16.16 format.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
In C, left-shifting a negative signed integer or shifting a value so large that it overflows the signed 32-bit range invokes undefined behavior. Multiplying `(int64_t)x * 65536` is well-defined for all signed values and safely produces a 64-bit value that can be checked and saturated to `Q16_MAX` or `Q16_MIN`.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping arithmetic, exceeding `INT32_MAX` causes two's-complement wrap-around to a huge negative number, suddenly reversing the sign of the control output. For a motor, that sudden sign flip drives full power in the opposite direction, causing violent mechanical jerks or instability, whereas saturation smoothly clamps the integral at maximum effort until the error reverses.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
For a 0–60 V battery voltage, Q16.16 is an excellent choice because 60 V fits easily within the ±32768 range and the 0.000015 V (15 µV) resolution is far finer than standard ADC voltage measurements. For GPS coordinates, Q16.16 is unsuitable because its 0.0000153° step size (~1.7 meters) is over 150 times too coarse for the required 0.0000001° (~1.1 cm) precision; a format with more fractional bits like Q8.24 is needed instead.