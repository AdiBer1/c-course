# Task 12 — Answers

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer: 
First, casting an unaligned byte buffer to a wider pointer can cause a hardware hard fault or alignment exception on strict embedded processors (unlike x86 PCs). Second, it depends entirely on the host CPU's native endianness, violating portability when parsing specific protocol streams.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?
Answer: 
Due to integer promotion, the expression promotes `uint8_t` to a signed `int` before shifting. Shifting into or past the sign bit causes undefined behavior, which UBSan flags as a shift overflow. Explicitly casting to `uint32_t` ensures unsigned arithmetic and prevents overflow.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.
Answer: 
Reading the big-endian bytes gives the 16-bit hex value `0xFF38`. Interpreted as a 16-bit signed two's complement integer (`int16_t`), the leading bits indicate a negative value, which evaluates numerically to $65336 - 65536 = -200$.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?
Answer: 
Calling explicit order-aware functions like `rd_u32_be()` makes the data contract clear, self-documenting, and portable. If the firmware is later ported to a big-endian architecture, assuming host-based byte swapping would break, whereas protocol-specific functions remain correct.