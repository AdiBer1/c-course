# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
A simple checksum can miss errors when changes in different bytes cancel each other out. For example, if one byte increases by 1 and another byte decreases by 1, the total sum stays the same, so the checksum does not detect the error. A CRC uses bit shifts and XOR operations, so it can detect many errors that produce the same simple sum.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
crc is a uint8_t, but in the expression crc << 1 it is promoted to int, so the result of the shift is an int. The CRC algorithm is supposed to keep the value 8 bits wide, so casting the result back to uint8_t explicitly tells the compiler that the narrowing is intentional. It also avoids an implicit conversion warning with -Wconversion.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
I would choose the bit-by-bit version when code size and memory usage are more important, or when the CRC is not performance-critical. It does not need an extra 256-byte lookup table and is simple to understand. I would choose the table-driven version when CRC calculations are frequent and performance is important, because it needs fewer operations per byte at the cost of extra memory.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice uses twice as much communication but does not provide the same structured error detection as a CRC. The CRC is designed to detect many common types of transmission errors, including single-bit and short burst errors. The sender can therefore send the data plus a small CRC value, and the receiver can independently calculate the CRC and detect whether the data was corrupted.
