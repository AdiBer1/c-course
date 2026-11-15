# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer: First, the cast needs p to be aligned. On some MCUs an unaligned read causes a HardFault, and in C it is undefined behavior anyway. Second, it reads the bytes in the CPU's own order, so it only returns the right value on a little-endian CPU and gives garbage on a big-endian one.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer: `uint8_t` is promoted to `int`, so the expression is an `int`. 0xFF << 24 doesn't fit in a signed int, and UBSan reported a signed shift overflow. Casting to `uint32_t` first makes the shift unsigned, which is well defined.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer: Combine the bytes: (0xFF << 8) | 0x38 = 0xFF38 = 65336. The top bit is set, so the value is negative. 65336 - 65536 = -200.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer: The byte order belongs to the protocol, not to the CPU. `rd_u32_be()` always gives the right result on any CPU and shows what the code means. A manual swap only works on little-endian and breaks (or swaps twice) when the code is ported to a big-endian chip.
