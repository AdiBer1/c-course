# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
A 12-bit signed number uses the highest bit as the sign bit. If this bit is 1, the number is negative. In my code, I check this bit and subtract 4096 (2^12) to get the correct negative value. For example, 0xF60 is 3936, and 3936 - 4096 = -160.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
`const` means that the function does not change the register value. `volatile` tells the compiler that the value can change outside the program, for example because of hardware. Without `volatile`, the compiler might optimize the read and use an old value instead of reading the register again.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
The register must be read only once because its value can change at any time. If we read it several times, different fields could come from different values. My code saves the register value in a local variable and then decodes all fields from that same value.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
`0x40021000u` is a memory address. `(volatile uint32_t *)` converts it to a pointer to a volatile 32-bit value. The `*` reads the value stored at that address. On a microcontroller, this address may belong to a hardware register. On a PC, this address is usually not mapped to memory that the program can access, so reading it can cause a crash.
