# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
In a 12-bit field, the leftmost bit is the sign bit. If it is 1, the number is negative.

Our code checks the sign bit using 0x800. If it is set, we subtract 0x1000 (4096) to get the correct negative value.

For example: 0xF60 = 3936 - 4096 = -160.
## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:

volatile tells the compiler that the value can change at any time, so it must read the register from memory.

Without volatile, the compiler might skip the read entirely.

const means the function cannot change the register's value.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
Because the value can change between the reads.

Reading it only once ensures that all fields are decoded from the same value.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
- 0x40021000u – The memory address of the register.
- (volatile uint32_t *) – Converts the address into a pointer to a 32-bit volatile value.
- * – Reads the value stored at that address.
It works on a microcontroller because this address is mapped to a hardware register.

On a regular PC, this address is usually not accessible to the program, so trying to read it may cause a crash.
