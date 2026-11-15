# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Each number is stored as its real value times 65536. So when we multiply two of them, the result is the real answer times 65536 twice. That number can get really big, much bigger than 32 bits can hold, so we use int64_t to have enough room. Then we divide by 65536 only once, because we still want the answer to be "times 65536" so it stays in Q16.16. If we divided by 65536 twice, we would lose the part after the decimal point.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
In C, shifting a negative number to the left is undefined behavior, which means the result is not guaranteed and the compiler can do anything. Also, if the number is too big (more than 32767), shifting it pushes it past the int32 limit and that is also a problem. So instead we multiply by 65536 using int64_t, and if the result is too big or too small we just cap it at the max or min.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping, when the number goes over the max it suddenly jumps to a huge negative number. So a motor that was going full speed forward could suddenly get a command to go full speed backward, which can break things or be dangerous. With saturation, the number just stays at the max, so the motor keeps going full forward. That is much safer and easier to predict.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
For battery voltage it is a good choice. 60 V fits easily in the range, and 0.000015 V is way more exact than we need. For GPS it is not a good choice. 180 fits in the range, but the smallest step (0.000015) is much bigger than the 0.0000001 we need, so it is not exact enough. For GPS we would need more bits for the fraction, or a different way to store it, like whole numbers times 10,000,000.
