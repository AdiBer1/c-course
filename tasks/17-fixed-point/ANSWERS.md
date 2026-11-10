# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Multiplying two 32-bit Q16.16 values produces a 64-bit result that exceeds INT32_MAX before scaling down, requiring int64_t to prevent intermediate overflow. The mathematical product is $(a / 65536) \times (b / 65536) = (a \times b) / (65536^2)$. To return to a single Q16.16 representation with denominator 65536, we divide by 65536 once so that the output remains $(a \times b / 65536) / 65536$.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
Left shifting (<< 16) a negative signed integer is Undefined Behavior in C. Additionally, left shifting large positive values can shift a 1 into or past the sign bit of a 32-bit integer, causing signed overflow. Multiplying by 65536LL after casting to int64_t avoids UB and handles large values safely.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
If arithmetic wraps around on overflow, a large integral term will wrap from maximum positive to maximum negative, causing a sudden full-reverse command to the motor (jerking/instability). Saturation holds the integral at maximum output, ensuring smooth motor drive and safe continuous operation.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
For battery voltage (0–60 V), Q16.16 is excellent because its range (±32768 V) and resolution (~0.000015 V) far exceed requirement. For GPS coordinates needing 0.0000001 degree resolution, Q16.16 is insufficient because its minimum step (0.000015) is 150 times too coarse; a format with more fractional bits (like Q8.24) is required.
