# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
1. Memory Alignment: Casting an arbitrary byte pointer `p` to `(const uint32_t *)` is unsafe because `p` may be unaligned (not divisible by 4). On microcontrollers like ARM, unaligned access can cause hardware faults (Bus Error / HardFault).
2. Endianness Dependency & Aliasing: Directly dereferencing as a `uint32_t` relies on the host CPU's native byte order rather than explicitly parsing Little-Endian, making the code unportable and violating C's Strict Aliasing rule.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
The expression evaluates to a 32-bit `signed int` due to C's integer promotion rules. Without a cast, shifting `0xFF` left by 24 bits shifts a 1 into the sign bit (bit 31), causing a signed integer overflow (Undefined Behavior) which UBSan flags. Casting to `(uint32_t)` ensures the shift is performed on an unsigned integer, where shifting into bit 31 is fully defined.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
1. In Big-Endian, `0xFF` is the MSB and `0x38` is the LSB.
2. Combining them yields the 16-bit value `(0xFF << 8) | 0x38 = 0xFF38`.
3. Interpreted as a signed 16-bit integer (`int16_t` in two's complement), the leading 1 bit indicates a negative number: `-(0x10000 - 0xFF38) = -(65536 - 65336) = -200`.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
Calling `rd_u32_be()` explicitly separates the protocol specification (Big-Endian) from the host hardware architecture. If the firmware is ever ported or tested on a Big-Endian system, `rd_u32_be()` will still work correctly, whereas hardcoded byte-swapping logic would break. It also makes the code self-documenting and alignment-safe.

