# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
It may cause an unaligned memory access, and the result depends on the CPU's byte order.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
The expression is an int. Shifting 0xFF by 24 causes signed integer overflow. Casting to uint32_t before shifting fixes it.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
0xFF38 is negative because its highest bit is 1. Converting from 16-bit two's complement gives 65336 - 65536 = -200.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
rd_u32_be always reads the protocol's byte order correctly, regardless of the CPU. It makes the code portable and easier to maintain.
