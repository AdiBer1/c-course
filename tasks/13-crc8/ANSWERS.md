# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
If two bytes are swapped, the sum remains the same, so a simple checksum can't detect the error And a CRC can detect this type of error.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
The `crc << 1` has type of int because of integer promotion. Casting the result to uint8_t is correct because the CRC is an 8-bit value, so only 8 bits is kept.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
I choose the bit-by-bit when memory is limited and I would choose a lookup table when performance is more important and there is enough memory available for it.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice requires more bandwidth and storage. A CRC add only a small amount of extra data while providing a error detection.
