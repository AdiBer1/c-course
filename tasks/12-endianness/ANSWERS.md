# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
Reason 1, byte order: this reads the 4 bytes in whatever order the current computer uses. My PC is little-endian, so it happens to work, but on a big-endian machine it gives a different number. The function is supposed to always read little-endian, no matter where it runs.
Reason 2, alignment: the buffer can start at any address, for example in the middle of a packet. Reading a uint32_t from an address that doesn't divide by 4 is undefined behavior, and on a Cortex-M0 it causes a HardFault and the device crashes. My PC just allows it, which is why the tests pass there.

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:
Before the shift, C turns the uint8_t into a signed `int`. So `p[3] << 24` is an int. With 0xFF, the result is 0xFF000000, which doesn't fit in a positive int because it hits the sign bit. That's undefined behavior. I cast to uint32_t from the start, so I didn't see the error, but UBSan would report something like "left shift of 255 by 24 places cannot be represented in type 'int'". Casting each byte to uint32_t before shifting fixes it, because the shift then happens on an unsigned 32-bit number, where it's well defined.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:
The value is big-endian, so the first byte is the high one.
1. Put them together: (0xFF << 8) | 0x38 = 0xFF38, which is 65336 as an unsigned number.
2. The top bit (bit 15) is on, because 65336 is 32768 or more. So it's really a negative number in two's complement.
3. Subtract 65536 (2 to the power of 16): 65336 - 65536 = -200.
That's what rd_i16_be does: it calls rd_u16_be, and if the result is 32768 or more, it subtracts 65536.

## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:
rd_u32_be builds the number with shifts, so it gives the right answer on any computer, little-endian or big-endian. It doesn't depend on the host at all. Code that says "swap the bytes because we are little-endian" only works as long as that's true. If the firmware moves to a different MCU, it silently breaks. It's also easy to swap twice or forget to swap in one place. And `rd_u32_be` says exactly what the data is: "this field is big-endian". Anyone reading the code understands it right away.

