# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
In a 12-bit two's complement number, bit 11 (`0x0800`) is the sign bit, representing -2048 instead of +2048. When stored in a 16-bit signed integer (`int16_t`), the upper four bits (bits 12 to 15) must replicate bit 11 to preserve the negative value. My code checks if bit 11 is set (`raw_temp & 0x0800u`), and if so, ORs it with `0xF000u` to extend the sign across the upper bits before casting to `int16_t`.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
The `volatile` qualifier tells the compiler that the value at that memory address can change asynchronously due to external hardware, even when the program does not modify it, while `const` ensures the firmware does not attempt to write to a read-only status register. Without `volatile`, the compiler might cache the value in a CPU register, reorder the read, or optimize away repeated reads altogether, causing the firmware to miss real-time hardware updates.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
Hardware registers can update independently at any instant between CPU instructions. Reading the register multiple times to extract different fields could result in combining bits from two different hardware states (such as reading a new FIFO level alongside an old ready or overrun flag), producing an inconsistent and corrupted snapshot.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
`0x40021000u` is the raw hexadecimal hardware address; `(volatile uint32_t *)` casts that integer into a pointer to a volatile 32-bit unsigned integer; and the leading `*` dereferences the pointer to perform an actual 32-bit memory read at that address. On a microcontroller without an MMU, peripheral registers are directly memory-mapped to fixed physical addresses, whereas on a PC, programs run in virtual memory protected by the operating system, so accessing an unmapped physical address triggers a segmentation fault.