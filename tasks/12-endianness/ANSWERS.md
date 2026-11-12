# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
First, byte order: the cast reads the 4 bytes in the CPU's OWN order. On a little-endian PC that happens to be little-endian, so the tests pass, but on a big-endian CPU the same code would return the bytes reversed. The function would no longer do what its name says. Second, alignment: `p` may point to any address, for example byte 1 of a received packet. A `uint32_t` access at an address that is not a multiple of 4 is a HardFault on a Cortex-M0, and on other CPUs it is slower or wrong. It is also undefined behaviour in C (both the misaligned access and reading `uint8_t` data through a `uint32_t` pointer, the strict aliasing rule), so the compiler may break it when optimizing. Reading byte by byte avoids all of these problems.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
Because of integer promotion, `p[3]` is converted to a signed `int` before the shift, so `p[3] << 24` has type `int`. For 0xFF the mathematical result is 0xFF000000 = 4,278,190,080, which does not fit in a 32-bit signed `int` (max 2,147,483,647): the 1 bits are shifted into the sign bit. That is signed overflow, undefined behaviour, and UBSan reports something like "left shift of 255 by 24 places cannot be represented in type 'int'". Writing `(uint32_t)p[3] << 24` converts the byte to an unsigned 32-bit value first, so the shift is done on an unsigned type where 0xFF000000 fits and everything is well defined.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
Big-endian means the first byte is the most significant. So the raw 16-bit value is `(0xFF << 8) | 0x38` = 0xFF38 = 65,336 as an unsigned number. The sensor uses two's complement, so the top bit (bit 15) is the sign. It is set here, so the number is negative, and in 16-bit two's complement the real value is raw - 2^16: 65,336 - 65,536 = -200. My `rd_i16_be` does exactly this: it reads the raw value with `rd_u16_be`, and if it is >= 0x8000, it subtracts 0x10000 before returning it as an `int16_t`.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
`rd_u32_be()` describes what the DATA is ("this field is big-endian"), not what the CPU is, so the code is correct on any CPU and does not need to change if the firmware is moved to a different chip. "Swap because we are little-endian" mixes two facts, the protocol's order and the CPU's order. It is easy to swap twice or forget to swap, and the code silently breaks on a big-endian chip. The byte-by-byte functions are also alignment-safe, and they make the code self-documenting: a reader immediately sees which byte order the protocol uses. On a modern compiler they are usually optimized into a single load-and-swap instruction anyway, so there is no cost.