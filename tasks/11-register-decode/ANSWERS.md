# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
A 12-bit temperature field uses two's complement representation.
If bit 11 is set, the number is negative.
To get its signed value, we subtract 4096 from the raw value.
For 0xF60, the raw value is 3936, so 3936 - 4096 = -160.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
const means the function cannot change the register through this pointer.
volatile tells the compiler that the register may change outside the program.
Without volatile, the compiler might optimize or reuse a register read instead of reading it again.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
The register must be read only once because its value may change while the program is running.
If we read it several times, different fields could come from different register values.
Reading once and saving the value ensures that all fields are decoded from the same value.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
The number 0x40021000 is a memory address.
The cast converts this address into a pointer to a volatile uint32_t.
The * dereferences the pointer and reads the value at that address.
On a microcontroller, this address may belong to a hardware register.
On a regular PC, this address is usually not mapped to accessible memory, so accessing it may cause a crash.
