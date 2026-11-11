# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
First, the buffer may not be aligned for a `uint32_t`, so reading it through a `uint32_t *` can cause an unaligned access and can even crash on some CPUs.
Second, the result depends on the CPU's own endianness. The function is supposed to explicitly read little-endian bytes, but the cast just interprets the bytes according to the host CPU's byte order.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
`uint8_t` is promoted to `int` before the shift, so `p[3] << 24` is performed as an `int` expression. With the value 0xFF, shifting it left by 24 can produce undefined behavior because the result does not fit in a signed `int`.
Casting to `uint32_t` before the shift makes the operation an unsigned 32-bit shift, so the high bit is handled correctly and the shift is well-defined.


## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
Because the value is big-endian, `FF` is the high byte and `38` is the low byte. We combine them as `(0xFF << 8) | 0x38`, which gives `0xFF38`.
Interpreting `0xFF38` as a signed 16-bit two's-complement value gives -200. The same 16 bits are therefore first read as an unsigned value and then interpreted as `int16_t`.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
`rd_u32_be()` describes the protocol directly: it says that the incoming value is big-endian. The code therefore does not depend on the CPU's own byte order.
This is clearer and more portable. If the firmware is later moved to a big-endian CPU, the same function still reads the protocol correctly without changing all the calling code.
