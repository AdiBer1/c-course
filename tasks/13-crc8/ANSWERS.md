# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
If two bytes change in opposite directions, their sum can stay the same. A CRC can detect this kind of error.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
The type is int because C promotes uint8_t to int. The cast to uint8_t is correct because CRC must stay 8 bits.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
I would use bit by bit when the memory is limited and the table-driven version when the speen is more important and there is enough memory

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice uses more space and does'nt detect every error. CRC is more efficient and can detect a lot of types of noise in data.

