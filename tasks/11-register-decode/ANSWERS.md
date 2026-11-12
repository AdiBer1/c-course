# Task 11 — Answers

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer: 
In a 12-bit two's complement representation, the most significant bit (bit 11) acts as the sign bit. When extracting this field into a 16-bit signed integer, sign extension ensures that negative values retain their correct negative sign by filling the upper bits with 1s. My code checks if the 12th bit is set, and if so, performs a bitwise OR with `0xF000u` to properly extend the sign.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer: 
The `volatile` qualifier informs the compiler that the value at this memory address can change asynchronously due to external hardware events. Without `volatile`, the compiler might optimize the code by assuming the value never changes, potentially caching an old read or omitting subsequent reads entirely.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer: 
Hardware registers can update dynamically or clear certain flags upon being read. Reading the register multiple times during a single decode operation could result in fetching inconsistent data from different moments in time, leading to corrupted or desynchronized state representation.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
 This expression casts the literal physical address `0x40021000u` into a volatile 32-bit unsigned pointer, and then dereferences it to read the hardware register value. This works on microcontrollers because they map peripherals directly to fixed physical addresses without an OS memory protection layer. On a PC, direct physical address access violates operating system memory protections, triggering a segmentation fault.