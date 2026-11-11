# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
The multiplication of two int32_t values can produce a value that does not fit in 32 bits, so I use int64_t for the intermediate result. Both numbers already have a factor of 65536 because they are stored in Q16.16. After multiplying, there are two such factors, so I divide by 65536 once to bring the result back to Q16.16. Dividing by 65536 twice would remove the fixed-point scaling completely.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
I avoided x << 16 because left-shifting a negative signed value is undefined in C. Large values can also overflow the 32-bit result. I used multiplication by 65536 in an int64_t instead, and then saturated the result to the Q16.16 range.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping arithmetic, the value can suddenly change from a very large positive value to a large negative value, or the other way around. In a motor controller this could cause a sudden and incorrect control signal. Saturation keeps the value at the maximum or minimum instead, so the controller does not suddenly jump to the opposite side.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
For a battery voltage from 0 to 60 V, Q16.16 can be a good choice because its range is large enough and its resolution is much smaller than 0.001 V. For GPS coordinates, the range is also large enough, but the resolution is not fine enough for a required accuracy of 0.0000001 degrees. A different fixed-point format with more fractional bits would be more suitable for the GPS case.
