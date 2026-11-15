# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer: Swapped bytes. {0x01, 0x02} and {0x02, 0x01} have the same sum, so a sum can't see the error. A CRC gives a different result because the position of each byte matters.


## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer: `crc << 1` is an `int`, because `uint8_t` is promoted to `int` in arithmetic. The bit that shifts out of the top should be dropped, since the CRC is only 8 bits wide. The cast to `uint8_t` does exactly that on purpose, so it isn't a hack.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer: Bit-by-bit when flash is small or the data rate is low, since it needs no table. Table-driven when speed matters (fast bus, lots of data) and 256 bytes of flash are available.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer: Sending the data twice doubles the size, and a noise burst can corrupt both copies in the same way. A one-byte CRC is much cheaper and catches all single-bit and short burst errors.
