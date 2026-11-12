# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
Each operand is a 32-bit number, so their product can need up to 62 bits plus the sign: for example 100.0 * 100.0 has raw values of about 6.5 million each, and their product is about 4.3 * 10^13, far beyond `int32_t`. Computing it in 32 bits would overflow (undefined behaviour for signed types) before we even get to scale it. So both operands are converted to `int64_t` first. About the scale: a = A * 65536 and b = B * 65536, so a * b = A * B * 65536 * 65536. The result we want is the Q16.16 form of A * B, which is A * B * 65536, so exactly ONE factor of 65536 has to be removed. Dividing by 65536 * 65536 would give the plain integer A * B and throw away all the fraction bits.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
In C, left-shifting a negative signed number is undefined behaviour, so `-1 << 16` is not guaranteed to give 0xFFFF0000, even if it usually does, and UBSan reports it. For large values, `x << 16` with x > 32767 pushes bits into or past the sign bit of `int32_t`, which is also undefined, and in practice wraps around to a completely wrong (even negative) number instead of saturating. I use `(int64_t)x * 65536` instead: multiplication of negative numbers is well defined, the 64-bit result cannot overflow, and then `saturate()` clamps anything outside -32768..32767 to Q16_MIN or Q16_MAX, as the header requires.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
If the motor cannot reach its target (for example it is blocked or the load is too big), the error stays positive and the integral keeps growing every millisecond. With wrapping arithmetic, it eventually passes INT32_MAX and jumps to a huge NEGATIVE value. The controller output suddenly flips from "full power forward" to "full power backward". The motor jerks or reverses violently, which can damage the mechanics or hurt someone. With saturation the integral stops at Q16_MAX: the output stays at "maximum forward", which is the closest correct answer, and once the error changes sign the controller recovers smoothly. In real controllers the integral is usually also clamped to a smaller limit (anti-windup), but saturation at least guarantees that no overflow ever reverses the output.

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
For a battery voltage, yes: 0-60 V fits easily in the ±32768 range, and a resolution of 0.000015 V (15 microvolts) is far finer than any ADC on a battery monitor can measure, so Q16.16 is more than enough. Even a format with fewer fraction bits would be fine. For a GPS coordinate, no: the range of ±180 fits, but the needed resolution of 0.0000001 degrees is about 150 times finer than Q16.16's step of 0.000015 degrees (which is about 1.7 m on the ground), so it would lose the precision that GPS provides. A better choice is a format with more fraction bits for the same 32 bits, for example storing degrees * 10^7 as an `int32_t` (±1,800,000,000 fits), which is what many GPS modules and protocols use, or Q8.24, or a 64-bit format.