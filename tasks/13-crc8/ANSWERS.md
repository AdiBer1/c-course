# Task 13 — Answers

## Q1. A simple checksum adds all bytes. Give an error that a sum does not detect but a CRC does.

Answer: 
A simple checksum is commutative, meaning it fails to detect byte reordering or swapping (e.g., changing `0x01, 0x02` to `0x02, 0x01`), because the arithmetic sum remains identical. CRC uses polynomial division and bit positions, making it fully sensitive to byte order and sequence changes.

## Q2. Inside `crc8_update`, `crc << 1` on a `uint8_t` produced a `-Wconversion` warning until you cast. What is the type of `crc << 1`, and why is the cast correct here rather than a hack?

Answer: 
Due to integer promotion rules in C, `crc << 1` promotes operands smaller than `int` to an `int`. Assigning or returning it as a `uint8_t` triggers a narrowing conversion warning. The explicit cast is correct because it formally documents the intentional truncation to an 8-bit register, enforcing the strict bit-width bounds of CRC-8.

## Q3. The bit-by-bit CRC needs 8 loop iterations per byte. A table-driven version uses a 256-byte lookup table and one step per byte. When would you choose each on a microcontroller?

Answer: 
Choose the bit-by-bit method when Flash memory (ROM) is severely constrained, as it eliminates the 256-byte lookup table overhead. Choose the table-driven version when execution speed and CPU cycles are critical, trading a small amount of ROM for significantly faster processing.

## Q4. Why does the receiver compute the CRC over the data and compare, instead of the sender just sending the data twice?

Answer: 
Sending data twice doubles bandwidth consumption and power usage, which is highly inefficient on constrained embedded links. Additionally, if a persistent noise source corrupts a transmission, sending it twice would likely repeat the exact same corrupted data, whereas CRC detects errors with minimal overhead (just one extra byte).