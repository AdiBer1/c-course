# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
Unaligned memory access: If p is not aligned to a 4-byte boundary, dereferencing a uint32_t * causes undefined behavior in C. While x86/x64 PCs handle unaligned loads in hardware, many embedded architectures (like older ARM cores) will trigger a hard fault or crash.

Endianness & Strict Aliasing: The pointer cast simply reads memory in the host CPU's native byte order, so it will return the wrong value on a big-endian system. Additionally, reading a byte buffer through a uint32_t * violates C's strict aliasing rules, which can cause optimizing compilers to miscompile the code.
## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
Because of C's integer promotion rules, uint8_t is promoted to a signed int (assuming 32-bit int), making the expression's type int. Shifting 0xFF left by 24 bits shifts a 1 into the sign bit of a signed int, which is undefined behavior in C; UBSan reports a runtime error (left shift of 255 by 24 places cannot be represented in type 'int'). Casting p[3] to (uint32_t) before shifting forces unsigned arithmetic, where shifting into the most significant bit is completely well-defined.
## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
In big-endian format, the most significant byte comes first, so FF and 38 combine into the 16-bit hexadecimal value 0xFF38. Interpreted as a signed 16-bit two's complement integer, the leading 1 bit (0xF...) indicates a negative number. Taking the two's complement—inverting the bits (0xFF38 $\rightarrow$ 0x00C7, which is 199) and adding 1—gives a magnitude of 200, resulting in -200.
## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
Calling rd_u32_be() explicitly documents the wire protocol's byte order in the code and makes the firmware portable across both little-endian and big-endian MCUs without needing #ifdef endianness checks. Furthermore, explicitly constructing the integer from bytes safely handles unaligned buffer pointers, whereas byte-swapping an unaligned 32-bit pointer still risks hardware faults and strict-aliasing bugs.
