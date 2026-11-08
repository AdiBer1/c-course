# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Each raw value is the real value times 65536. Two raw 32-bit values can
multiply to a number up to about 2^62, which does not fit in int32_t, so
the product must be computed in int64_t to avoid overflow. The raw
product is (a x 65536) * (b x 65536) = a*b x 65536^2, but the result
should be a*b x 65536 (one factor of 65536). So I divide by 65536 once,
not by 65536 * 65536, which would remove the fraction bits completely.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
Shifting a negative signed number left is undefined behavior in C, and
for large values the shift overflows int32_t (any x >= 32768 or
x < -32768 no longer fits after << 16), which is also undefined for a
signed type. It also would not saturate, so the result would be garbage.
Instead I multiply by 65536 in int64_t, which is always well defined,
and then saturate the result to Q16_MAX or Q16_MIN.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping arithmetic, when the integral passes the maximum value it
wraps around to a large negative number, so the control output suddenly
flips from full positive to full negative. A motor would jump from full
speed in one direction to full speed in the other, which can damage the
mechanics or be dangerous. With saturation, the integral stays at the
maximum, so the output stays at its limit and changes gradually as the
error changes. Saturation fails in a safe, predictable way.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
Battery voltage 0-60 V: yes. The range of +-32768 easily contains 60 V,
and a resolution of 0.000015 V is far finer than any ADC reading, so
Q16.16 is a good choice. GPS coordinate: no. The range of +-180 fits, but
0.000015 degrees is about 1.7 meters (one degree is about 111 km), while
0.0000001 degrees (about 1 cm) is needed. Q16.16 is too coarse. A better
choice is more fraction bits and fewer integer bits, for example Q9.23
(range +-256, resolution about 0.00000012), or an int32_t that stores
the coordinate in units of 1e-7 degrees.
