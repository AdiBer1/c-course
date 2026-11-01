# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
A 12-bit two's complement number uses its top bit (bit 11, 0x800) as the
sign. If that bit is 1, the number is negative, and its value is the raw
value minus 4096 (2^12). 0xF60 is 3936, and 3936 - 4096 = -160. In my code
I read the 12 bits into an int32_t, check `temp & 0x800`, and if it is set
I subtract 0x1000. Without this, the 12-bit value would be read as a big
positive number.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
volatile tells the compiler that the register can change at any time by
the hardware, not only by my code. Without it, the compiler may assume the
value does not change, keep it in a CPU register, merge two reads into one,
or remove a read whose result looks unused. Then my code would work with an
old value instead of the real hardware state. const means that this
function only reads the register and never writes to it.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
The hardware can change the register between two reads. If I read it once
for the mode and again for the temperature, the two values could come from
different moments, and the decoded struct would mix old and new data.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
0x40021000u is a fixed address (a number). (volatile uint32_t *) casts
this number to a pointer to a volatile 32-bit value. The * in front
dereferences it, so the whole expression means "the 32-bit register that
sits at address 0x40021000". On a microcontroller this address is mapped to
a real hardware register, so reading it gives the chip's state. On a PC the
operating system gives each program its own virtual memory, and this address
is not mapped for it, so accessing it causes a segmentation fault.
