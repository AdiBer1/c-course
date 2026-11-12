# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
We need int64_t because multiplying two 32-bit numbers can produce a result that needs 64 bits. We divide by 65536 to convert the result back to Q16.16 format, keeping 16 fractional bits.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
Left-shifting a negative signed number is undefined behavior in C. Large values can also overflow the 32-bit range. Multiplication by 65536 using int64_t is safer because we can check the result and apply saturation.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping arithmetic, the integral can suddenly change from a large positive value to a negative value when it overflows. This can cause the motor to behave unpredictably. Saturation keeps the value within the allowed range and makes the system safer.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
Q16.16 is a good choice for battery voltage because its range and resolution are sufficient for values between 0 and 60 volts. It is not a good choice for GPS coordinates because the required precision is much higher than its resolution. A format with more fractional bits would be better for GPS.

