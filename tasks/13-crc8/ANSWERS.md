# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
Swapping two bytes. The frame {0x01, 0x02} and the corrupted frame {0x02, 0x01} have the same sum (0x03), so a sum checksum accepts the wrong data. Addition does not care about order. A CRC processes the bytes in sequence and each byte is shifted and mixed into the state before the next one arrives, so the order changes the result: crc8({0x01,0x02}) != crc8({0x02,0x01}). The same goes for two errors that cancel out in a sum (one byte +1, another byte -1).

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
Because of integer promotion, `crc` is promoted to `int` before the shift, so `crc << 1` is an `int` with a value up to 0x1FE (9 bits). Assigning it back to a `uint8_t` narrows it, which is what -Wconversion warns about. The cast is correct because throwing away bit 8 is part of the algorithm ("keep crc 8 bits wide"): the bit that falls off the top corresponds to the x^8 term of the polynomial, and it has already been handled by checking 0x80 and XORing with 0x07. Conversion to an unsigned type is well defined (modulo 256), so the cast just states explicitly a truncation we actually want.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
Bit-by-bit when flash is tight and the data is small or infrequent, e.g. checking a 2-byte sensor reading a few times a second: the extra cycles do not matter and it costs almost no memory. Table-driven when throughput or timing matters, e.g. checking long packets at high data rates or inside an interrupt with a tight deadline, and you can afford 256 bytes of flash (the table should be `const` so it stays in flash, not RAM). It is a classic speed vs. memory trade-off.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice doubles the bandwidth, while a CRC-8 adds only one byte regardless of message length. Duplication is also weaker: if the two copies differ you know there is an error but not which copy is right, and an error that hits both copies the same way (e.g. a stuck bit on the line at the same position) goes undetected. A CRC gives guaranteed detection of all single-bit errors and all bursts up to 8 bits, at a fraction of the cost.