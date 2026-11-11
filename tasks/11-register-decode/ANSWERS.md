# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
If bit 11 is 1, the 12-bit value is negative. I subtract 4096 to get the correct signed value.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
const prevents changing the register through the pointer. volatile tells the compiler to read the actual hardware value and not optimize the access away.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
The register can change at any time. Reading it once ensures all fields come from the same value.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
It casts a fixed memory address to a volatile uint32_t pointer and reads its value. It works on a microcontroller because the address belongs to hardware, but usually causes a crash on a PC because that address is not accessible.
