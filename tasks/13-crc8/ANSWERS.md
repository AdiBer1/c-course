# Task 13 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer:
Swapping two bytes: {0x01, 0x02} and {0x02, 0x01} have the same sum, so a
sum does not notice, but a CRC gives different results because the order
of the bytes matters. Another example is two errors that cancel each
other (one byte +1 and another byte -1).

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer:
crc is promoted to int before the shift, so crc << 1 has type int
and can be bigger than 255. Assigning it to a uint8_t narrows it
implicitly, and -Wconversion warns. The cast (uint8_t) is correct
here, not a hack, because I want exactly this: the CRC is an 8-bit value,
and the bit that falls off the top is supposed to be discarded (the
polynomial XOR already accounts for it). The cast makes that intent
explicit.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer:
Bit-by-bit needs no table, so it uses almost no memory, but it is slower
(8 iterations per byte). I would use it on a very small MCU with little
flash, or when messages are short and CRC speed does not matter. The
table version costs 256 bytes of flash but does one step per byte, so I
would use it when speed matters, for example with large frames, high data
rates, or when the CPU has many other tasks.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer:
Sending the data twice doubles the size of every message, while a CRC
adds only one byte. Also, the copies could be corrupted in the same way
(for example by a burst of noise that hits both), and then they would
still match. A CRC is designed mathematically to catch single-bit errors
and burst errors up to its length, with very little overhead.
