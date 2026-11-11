# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
Casting p to uint32_t * (1) can cause an unaligned access, which crashes (HardFault) on some MCUs such as Cortex-M0, and (2) depends on the CPU’s byte order, so it returns reversed bytes on a big-endian machine.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
p[3] is promoted to a signed int, so 0xFF << 24 does not fit in int and UBSan reports left shift of 255 by 24 places cannot be represented in type 'int'. Casting to uint32_t first makes it an unsigned shift, which is well defined.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
Big-endian FF 38 gives 0xFF38 = 65336. Read as a 16-bit two’s complement value (top bit set), it is 65336 - 65536 = -200.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
rd_u32_be() states the intent (the protocol is big-endian) and works on any CPU, while “swap because we are little-endian” silently breaks when the code is ported to a different byte order.

