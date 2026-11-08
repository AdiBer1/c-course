# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Both operands use 16 fractional bits, so multiplying them can require 64 bits. We use int64_t to prevent overflow during the multiplication. The result has 32 fractional bits, so we divide by 65536 to return to 16 fractional bits.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
Left-shifting a negative signed number is undefined behavior in C. Large values can also overflow the 32-bit range. Multiplying by 65536 using int64_t and checking the limits is safer.


## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping arithmetic, the value can suddenly change from a large positive number to a negative number, or the other way around. This can cause the motor controller to make incorrect adjustments. Saturation keeps the value within its valid range and prevents this sudden change.
## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
Q16.16 is a good choice for battery voltage because its range and resolution are sufficient for values between 0 and 60 volts. It is not a good choice for GPS coordinates because its resolution is not precise enough for 0.0000001 degrees. A format with more fractional bits would be better for GPS coordinates.
