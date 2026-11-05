# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
First, it is not portable because the result depends entirely on the host CPU's native endianness. Second, casting an unaligned byte pointer to a 32-bit integer pointer can cause a hardware fault (Hard Fault) and crash the program on microcontrollers that require strict memory alignment.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
In C, small types like uint8_t are automatically promoted to a signed int before shifting. Shifting into or past the sign bit causes signed integer overflow, which is undefined behavior that UBSan detects and reports. Explicitly casting to uint32_t before shifting prevents signed promotion and avoids the overflow.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
In big-endian order, the bytes FF 38 combine into the 16-bit hex value 0xFF38. Interpreted as a signed 16-bit integer using two's complement, this bit pattern represents the decimal value -200.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
Calling explicit protocol-specific functions like rd_u32_be() makes the intended data format clear and independent of the host CPU. Relying on assumptions about the host's endianness makes the code fragile and prone to breaking if ported to a different architecture.
