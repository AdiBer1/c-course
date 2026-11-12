# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
A simple sum does not detect swapped bytes (e.g., receiving `{0x20, 0x10}` instead of `{0x10, 0x20}` yields the exact same sum of `0x30`) or two bit errors that cancel each other out (+1 in one byte and -1 in another). Because a CRC shifts bits and XORs them with a polynomial at every step, it is sensitive to byte ordering and catches swapped bytes and burst errors.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
Due to C's integer promotion rules, `crc << 1` promotes the `uint8_t` operand to `int` before shifting, resulting in an `int` type that can hold a 9th bit if bit 7 was set. Casting `(uint8_t)(crc << 1)` is mathematically correct here—not a hack—because the 8-bit CRC algorithm explicitly requires discarding the shifted-out 9th bit and keeping only the lower 8 bits.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
The bit-by-bit version uses almost no Flash/RAM and is best on tiny, memory-constrained microcontrollers or when transmitting small messages infrequently (like reading a sensor once per second). The 256-byte lookup table version trades 256 bytes of ROM/RAM for roughly 8x faster execution, making it ideal for high-speed communication protocols or large data buffers where CPU time is critical.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice adds 100% bandwidth overhead, doubling transmission time and power consumption, whereas a CRC adds only a single byte regardless of message length. Additionally, if a persistent hardware fault or noise pattern corrupts both transmissions identically, comparing two copies would falsely accept bad data, while a CRC mathematically verifies data integrity against the polynomial.