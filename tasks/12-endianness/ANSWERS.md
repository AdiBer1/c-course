# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
There are two problems with this approach. First, the pointer may not be properly aligned for accessing a uint32_t, which can cause undefined behavior or hardware problems on some CPUs. Second, the result depends on the host computer's endianness, so it may not work correctly on a big-endian CPU.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
A uint8_t is promoted to int before the shift operation. Since 0xFF << 24 exceeds the maximum value of a 32-bit signed int, the operation causes undefined behavior. Casting p[3] to uint32_t before shifting makes the operation unsigned and allows the result to be represented correctly.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
First, I combine the bytes in big-endian order: 0xFF38. As an unsigned 16-bit value, this is 65336. Since the value is signed and uses two's complement, I subtract 65536, which gives -200.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
Using rd_u32_be() makes the code independent of the host CPU's endianness. It also makes the code easier to understand and maintain because the conversion logic is centralized in one function. This way, the same code works on different CPUs without manually checking or swapping bytes.
