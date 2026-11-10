# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
Casting p directly to const uint32_t * and dereferencing it is a bad idea for two main reasons:

Alignment: The buffer p isn't guaranteed to start at an aligned address (which test_read_from_unaligned_address specifically checks). On many microcontrollers, reading a 4-byte value from an unaligned address triggers a crash (HardFault), and in C it's undefined behavior.

Endianness: The result depends on the CPU's byte order rather than the data's format. On a big-endian processor, that exact same line would return a completely different number, meaning the code might seem to work on one machine and silently break on another.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
Before the shift, uint8_t is automatically promoted to a signed int. Shifting 0xFF << 24 produces 0xFF000000, which exceeds the maximum positive value of a 32-bit signed int. Without the cast, UBSan would report an error along the lines of left shift of 255 by 24 places cannot be represented in type 'int'. Casting (uint32_t)p[3] forces the shift to happen in an unsigned 32-bit type, where the value fits without any issue.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
In big-endian order, 0xFF is the high byte and 0x38 is the low byte, giving the 16-bit value 0xFF38, which is 65336 in unsigned decimal. Since the most significant bit is set, interpreting it as a signed int16_t makes it negative: 65336 - 65536 = -200.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
Calling rd_u32_be() explicitly describes the format of the incoming data (big-endian) without making any assumptions about the host CPU, so the same code works correctly on any architecture. Writing code that just "swaps bytes because we're on little-endian" will silently break if the code is ever ported to a different processor. It's also harder to read and much easier to mess up—either by forgetting to swap in one place or accidentally swapping twice.

