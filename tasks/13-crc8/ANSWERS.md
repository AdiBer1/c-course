# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
Swapped bytes: the message `01 02` and the corrupted `02 01` have the same sum (3), because addition does not care about order, so a sum checksum accepts the wrong message. A CRC processes the bytes in order through the shift register, so the position of each byte matters: `crc8({01, 02})` and `crc8({02, 01})` are different, and the swap is detected. Another example is two errors that cancel out, like one byte +1 and another byte -1: the sum stays the same, but a CRC-8 catches all single-bit errors and all burst errors up to 8 bits long.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
Because of integer promotion, `crc` is converted to `int` before the shift, so `crc << 1` (and `(crc << 1) ^ 0x07`) has type `int`. Its value can be up to 9 bits, for example 0x80 << 1 = 0x100. Storing it back into a `uint8_t` drops the upper bits, which is an implicit narrowing conversion, so `-Wconversion` warns. The cast is correct because dropping the 9th bit is exactly what the algorithm requires: the header says "keep crc 8 bits wide", and the bit that is shifted out is the one we already tested (0x80) before the shift. The cast does not hide a bug. It states clearly that the truncation is intentional.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
Bit-by-bit uses almost no memory (no table, just a few instructions), but it is about 8 times slower per byte. I would use it on a very small MCU with little flash, or when only a few bytes need checking, for example the 2 data bytes of a Sensirion sensor reading a few times per second, where speed does not matter. The table-driven version costs 256 bytes of flash (the table can be `const` so it stays out of RAM) but needs only one lookup and one XOR per byte. I would choose it when a lot of data passes through the CRC, for example long radio or UART packets at a high data rate, or when the CRC runs inside an interrupt and must be as fast as possible.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice doubles the size of every message, while a CRC-8 adds only one byte no matter how long the message is, so it uses much less bandwidth and energy, which matters on slow buses and battery devices. Sending twice is also weaker: if the same noise burst hits both copies in the same way, or the receiver just compares the two copies and they differ, it still cannot tell which one is right. A CRC is designed to detect the typical transmission errors (single bit flips, bursts, swapped bytes) with high probability, using only a small fixed overhead.