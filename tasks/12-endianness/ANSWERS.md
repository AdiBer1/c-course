# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
First, alignment: the buffer may start at any address. On some MCUs
(like Cortex-M0) reading a 32-bit value from an unaligned address causes
a HardFault, and on others it is slow. Second, byte order: a cast reads the
bytes in the CPU's own order. On a little-endian PC it happens to give the
right result, but on a big-endian CPU the same code returns the bytes
reversed. The test passes on my PC only by luck.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
p[3] is a uint8_t, which is promoted to int before the shift, so the
type of the expression is int (signed). 0xFF << 24 is 0xFF000000, which
is bigger than the largest int, and shifting a value into the sign bit of
a signed int is undefined behavior. UBSan reported something like "left
shift of 255 by 24 places cannot be represented in type 'int'". The cast
(uint32_t)p[3] << 24 makes the shift happen on an unsigned 32-bit
value, where this is well defined.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
Big-endian means the first byte is the high byte, so I build
(0xFF << 8) | 0x38 = 0xFF38, which is 65336 as an unsigned number. Bit 15
is set, so as a signed 16-bit two's complement value it is negative: I
subtract 65536 and get 65336 - 65536 = -200.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
rd_u32_be() says exactly what the protocol is, and it works the same on
any CPU, little or big endian. A "swap because we are little-endian"
assumes something about the CPU, so when the code is moved to another
MCU it silently breaks, or needs #ifdef s to choose when to swap. With
rd_u32_be() the byte order of the protocol is in one place and the
CPU's order does not matter.
