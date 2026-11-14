# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
The temperature has 12 bits, and the top one (bit 11) is the sign bit. In two's complement that bit is worth -2048, not +2048. 0xF60 has bit 11 on, so it's not 3936, it's 3936 - 4096 = -160. If I just copy the 12 bits into an int16_t, the upper bits stay 0 and I get 3936, which is wrong. In my code I take the 12 bits out, check bit 11, and if it's on I subtract 4096. That gives the right negative number.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
`volatile` means "this value can change by itself", because the hardware changes it, not my code. So the compiler has to really read it every time I wrote a read. `const` means I only read the register, I never write to it. Without `volatile`, the compiler thinks nobody else touches this memory. Then it can read it once and keep using the old value, join two reads into one, or skip the read if it thinks it's not needed. In a loop that waits for READY, it could read only once and get stuck forever.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
The hardware can change the register between two reads. If I read it again for every field, I might get READY from one moment and FIFO_LEVEL from a later moment. Then my struct shows a state that never really happened. Also, on some chips reading a register clears some flags, so a second read can lose information. So I read it once into a local variable and take all the fields from that one copy.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
`0x40021000u` is just a number. `(volatile uint32_t *)` says: treat this number as the address of a 32-bit value that can change by itself. The `*` at the start reads the value at that address. On a microcontroller this works, because the datasheet says the sensor's register really sits at that address, and the program uses real physical addresses. On a PC every program has its own virtual memory, and this address doesn't belong to my program. So when I try to read it, the OS stops the program with a segmentation fault.

