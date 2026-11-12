# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
If two bytes are swapped, the checksum sum stays the same.
A CRC can detect this error because the order of the bytes matters.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
The type of crc << 1 is int because of integer promotion.
The cast is correct because CRC is an 8-bit value, and we want to keep only 8 bits.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
I would choose the bit-by-bit algorithm when memory is limited.
I would choose the table-driven version when speed is more important and there is enough memory.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice uses more memory and bandwidth.
A CRC adds only a small checksum and helps detect transmission errors.

