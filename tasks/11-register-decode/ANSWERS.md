# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
The temperature field uses a 12-bit signed integer in two's complement format, where bit 11 represents the sign. A value like 0xF60 has bit 11 set to 1, indicating a negative number. To convert this 12-bit signed value into a standard 16-bit signed integer (`int16_t`), sign extension must fill the upper 4 bits with 1s (turning 0xF60 into 0xFF60, which equals -160). In code, this can be achieved by casting after left-shifting by 4 and arithmetic right-shifting by 4.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
The `volatile` qualifier informs the compiler that the value at the register memory address can change at any time due to external hardware events. Without `volatile`, the compiler's optimizer might cache the value in a CPU register or omit subsequent reads entirely, failing to notice hardware status updates.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
Hardware status registers can update asynchronously between clock cycles. Reading the register multiple times to decode different fields could result in combining bits from two different hardware states, causing inconsistent or corrupted status interpretations. Reading it exactly once guarantees an atomic snapshot.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
`0x40021000u` is an unsigned raw memory address. `(volatile uint32_t *)` casts this address into a pointer to a volatile 32-bit unsigned integer, and the outer `*` dereferences it to read/write the register directly. On a bare-metal microcontroller, peripheral hardware registers are directly mapped to physical addresses (MMIO). On a PC operating system with virtual memory protection, accessing an arbitrary raw physical address triggers a segmentation fault crash.