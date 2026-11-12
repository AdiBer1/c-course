# Task 17 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why does `q16_mul` need an `int64_t` intermediate, and why is the result of `a * b` then divided by 65536 (and not by 65536 * 65536)?

Answer:
a and b are 32-bit numbers, so a * b can need up to 64 bits. If we multiply in 32 bits, the result overflows. That is undefined behaviour for signed numbers and we lose the real answer. So we first cast to int64_t and multiply there.
About the division: a is really A * 65536 and b is really B * 65536. So a * b = A * B * 65536 * 65536. The result we want in Q16.16 is A * B * 65536, so we have one 65536 too many. That's why we divide by 65536 one time. If we divided by 65536 * 65536 we would get a normal integer and not Q16.16, and we would lose all the fraction.

## Q2. `x << 16` looks like the natural way to convert an integer to Q16.16. Why did you (or should you) avoid it for negative numbers and large values?

Answer:
In C, shifting a negative number to the left is undefined behaviour, so the compiler can do anything with it. For large values, like 40000, the result doesn't fit in int32_t, and that is also undefined (signed overflow). Even when it "works", we get a wrong number and not saturation. So I multiply by 65536 in int64_t, where nothing overflows, and then saturate to Q16_MAX or Q16_MIN.

## Q3. A PID controller computes `integral += error * ki` every millisecond. What happens with wrapping (non-saturating) arithmetic when the integral gets large, and why is saturation the safer behaviour for a motor?

Answer:
With wrapping, when the integral goes past the biggest positive value it jumps to a very big negative value. So the controller goes from "full power forward" to "full power backward" in one millisecond. For a motor this is a big jerk and can break the mechanics or hurt someone. With saturation, the integral just stays at the maximum. The motor stays at full power, which is wrong maybe, but it is stable and expected, and the controller can come back down when the error changes. (A real PID also needs anti-windup to limit the integral.)

## Q4. Q16.16 has a resolution of about 0.000015 and a range of ±32768. For a battery voltage in volts (0–60 V) or a GPS coordinate in degrees (±180, needs 0.0000001), is Q16.16 a good choice? Explain.

Answer:
For battery voltage, yes. 60 V is far inside the range, and steps of 0.000015 V are much finer than any real ADC can measure.
For GPS, no. The range of ±180 is fine, but the resolution is not enough. One step is about 0.000015 degrees, which is about 1.7 meters on the ground, and we need 0.0000001 degrees (about 1 cm). It's better to use a plain int32_t in units of 0.0000001 degrees. 180 * 10,000,000 = 1,800,000,000, which still fits in int32_t. Many GPS modules and protocols do exactly this.