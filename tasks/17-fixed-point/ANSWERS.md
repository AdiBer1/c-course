# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Multiplying two 32-bit Q16.16 numbers produces a 64-bit intermediate value to avoid overflow, as the integer portion scales up and fractional bits double from 16 to 32 bits (Q32.32 format). Dividing the 64-bit product by 65536 ($2^{16}$) scales the result back down to Q16.16 by discarding the lower 16 extra fractional bits while maintaining precision.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
In C (prior to C23), left-shifting a negative signed integer is undefined behavior. Additionally, left-shifting large positive integers can overflow the 32-bit signed range (causing wrapping/undefined behavior). Multiplying by `Q16_ONE` after range-checking or casting to 64-bit safely converts both negative and large values without undefined behavior.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping arithmetic, an accumulating integral that exceeds `INT32_MAX` wraps around to `INT32_MIN`, causing the motor control output to instantly flip from maximum forward power to maximum reverse power. Saturating arithmetic clamps the output at `Q16_MAX`, keeping the motor running safely at its upper power limit without dangerous sudden reversals.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
Q16.16 is an excellent choice for a battery voltage (0–60 V) because 60 V fits easily within ±32768 V and a resolution of 0.0015% (15 µV) far exceeds sensor accuracy. However, it is a poor choice for high-precision GPS coordinates, where its resolution of ~0.000015° (~1.7 meters on Earth) is too coarse compared to the required 0.0000001° (~1 cm precision); a format like Q8.24 or Q12.20 would be better suited.

