# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
In a 12-bit two's complement field, if the highest bit (bit 11) is 1, the number is negative. Sign extension fills the upper bits of the 16-bit integer with 1s so the negative value is preserved. Our code checks if bit 11 is set and manually sets the upper bits.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
It uses volatile to tell the compiler that the hardware can change the register value at any time. Without volatile, the compiler might optimize and read the memory only once or cache it in a register.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
Because hardware registers can change between reads. Reading it multiple times could capture inconsistent states where different fields come from different moments in time.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
It casts the constant address 0x40021000u to a volatile pointer to a 32-bit unsigned integer, and then dereferences it to access the hardware register. It works on a microcontroller because that address maps directly to physical hardware registers, but on a PC, accessing arbitrary physical memory causes a segmentation fault.
