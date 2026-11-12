# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
A simple sum doesn't care about order. If two bytes swap places, like 01 02 becoming 02 01, the sum is the same (3 both times), so the error isn't caught. The same goes for one byte going up by 1 and another going down by 1. A CRC mixes every bit with the bits around it through shifts and XOR, so changing the order or the bits changes the result, and the error is caught.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
Before shifting, C turns the uint8_t into an int, so `crc << 1` is an int and can have 9 bits. Putting it back into a uint8_t cuts off the top bit, and -Wconversion warns about that. But in CRC-8 we actually want that: the bit that falls off the left side is supposed to be thrown away, and the algorithm already used it (it decided whether to XOR with the polynomial). So the cut is part of the algorithm, not a hack. In my code I do the math in a uint32_t, keep the low 8 bits with `& 0xFF`, and cast to uint8_t at the end, so it's clear the cut is on purpose.
## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
The bit-by-bit version is slower (8 loop steps per byte) but needs no extra memory. The table version is much faster (one step per byte), but the table takes 256 bytes of flash. I'd pick bit-by-bit on a small MCU with little flash, or when there isn't much data, like a short sensor message now and then. I'd pick the table when there's a lot of data at high speed, like a fast serial link, and there's flash to spare.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice doubles the size of every message, so the link is twice as slow. A CRC only adds one byte. Also, if the two copies don't match, you still don't know which one is right. And if the same error hits both copies the same way, it isn't caught at all. With a CRC the receiver just calculates it on its own and compares it to the byte at the end. That's cheap and catches almost all real errors.