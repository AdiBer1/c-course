# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
A simple sum cannot detect byte-swapping errors (e.g., receiving {0x01, 0x02} instead of {0x02, 0x01}) because addition is commutative. A CRC processes bits positionally using shifts and polynomial division, so order changes completely alter the final CRC result.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
In C, integer promotion converts operands smaller than int to int before bitwise shift operations, making crc << 1 an int. Assigning an int back to uint8_t triggers a -Wconversion warning. The cast is correct because we intentionally discard overflow bits above bit 7 as required by the 8-bit CRC algorithm.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
The bit-by-bit approach is chosen on resource-constrained microcontrollers with very small flash memory to save 256 bytes of storage. The table-driven approach is chosen when high throughput or low latency is required, trading memory space for 8x faster execution.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending data twice doubles the required bandwidth and transmission time, and noise during transmission could corrupt the exact same bit position in both copies. A CRC appends only 1 byte of overhead while offering strong mathematical guarantees to detect multiple bit errors.
