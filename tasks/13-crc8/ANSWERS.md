# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
Swapping two bytes. The sum of 0x01 + 0x02 equals the sum of 0x02 + 0x01, so a plain sum cannot see the error. In a CRC every byte affects the internal state according to its position, so changing the order changes the result. Another example: one bit flipping from 0 to 1 while another bit of the same weight in a different byte flips from 1 to 0. The sum stays the same, but the CRC changes.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
In C, types smaller than `int` such as `uint8_t` go through integer promotion to `int` before any operation, so `crc << 1` has type `int` and can be as large as 510. The cast `(uint8_t)` truncates it back to 8 bits, which is exactly what the algorithm requires: the top bit that was shifted out has already been handled and must be discarded ("keep crc 8 bits wide"). So the cast is part of the algorithm's definition and not just a way to silence the warning. It also makes the narrowing explicit and visible to the reader.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
The bit-by-bit version is small, with no table, so it suits chips with little flash or RAM, or cases where the CRC is computed rarely and over little data. It is slower, with 8 iterations per byte. The table-driven version is faster, with one step per byte, so it suits large amounts of data or strict timing, for example a fast data stream or a computation inside an interrupt. The cost is 256 bytes of constant memory, which can be placed in flash.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
A CRC is a single byte, while sending the data twice doubles the whole message, so the CRC saves bandwidth and time. Sending twice is also weaker: if both copies are damaged in the same way (for example repeating noise or a stuck line), they still look identical and the error goes unnoticed. And when the two copies differ, there is no way to tell which one is correct. A CRC has mathematical guarantees: it detects every single-bit error and every burst error up to 8 bits long.