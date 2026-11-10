# Task 12 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `return *(const uint32_t *)p;` a bad way to implement `rd_u32_le`, even if it passes the tests on your PC? Give two separate reasons.

Answer:
1. It works fine on a computer that stores numbers in little-endian order. But on a
   processor that stores numbers in big-endian order, it reads the bytes in the order
   the processor stores them, not in the order the data actually uses.

2. If the data is not aligned to a memory address that divides by 4, the processor
   may not be able to read the 32 bits in one access, and it may crash. Reading the
   buffer in small steps of 8 bits (one byte at a time) solves this difficult and
   dangerous problem, because a single byte can be read from any address. 

## Q2. `p[3] << 24` where `p[3]` is a `uint8_t` 0xFF: what is the type of the expression, and what did UBSan report if you didn't cast? How does the cast fix it?

Answer:

The type during the calculation is int. There will be a warning that during the shift
of 24 the sign bit can be overwritten, and then a wrong value and unexpected behavior
will happen. The conversion to uint32 solves the problem because there is no sign bit
there, so there is plenty of room for all the bits and all the problems are solved.

## Q3. A temperature sensor sends its value big-endian as bytes `FF 38`. Walk through how you get -200 from those two bytes.

Answer:


## Q4. Your firmware runs on a little-endian MCU and talks to a big-endian protocol. Why is it still better to call `rd_u32_be()` everywhere than to write "swap the bytes because we are little-endian"?

Answer:

It might be a problem, because another time we may want to run this program on
another MCU that runs in BE, and then the code will change the order of the bytes
for no reason, even though it is not needed. This will be a terrible confusion,
because it will turn it into LE even though it really should be BE.

If we use `rd_u32_be()`, it will not be a problem, because it will always represent
the number in the order we want, because it does not depend on the order of the MCU.