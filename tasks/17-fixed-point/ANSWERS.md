# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Multiplying two 32-bit Q16.16 values produces a 64-bit product because each operand is scaled by $2^{16}$, meaning their product is scaled by $2^{32}$. Using an int64_t prevents overflow during this multiplication. We divide by 65536 ($2^{16}$) rather than $2^{32}$ because we only want to remove one factor of $2^{16}$ to bring the resulting value back into the standard Q16.16 scale.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
Left-shifting a negative signed integer is technically undefined behavior in C, and large values will overflow the 16-bit integer boundary before or during the shift, destroying the sign bit or producing incorrect results. Instead, multiplying by Q16_ONE using a wider type like int64_t safely handles negative numbers and avoids undefined behavior.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping arithmetic, a large integral that exceeds the maximum limit silently overflows and wraps around to a large negative number, causing a sudden and catastrophic reversal in control output. Saturation prevents this by clamping the value at the maximum limit, keeping the motor stable and at maximum output rather than erratically reversing.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
Q16.16 is a great choice for battery voltage (0–60 V) because the range easily fits within $\pm32768$ and the resolution of $\approx 0.000015$ provides more than enough precision. However, it is a poor choice for GPS coordinates because the required precision of $0.0000001$ far exceeds the fixed-point resolution, and the $\pm180$ range would waste the upper bits while lacking the required decimal places.

