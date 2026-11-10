# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
In 12-bit two's complement the top bit (bit 11) is the sign bit. If it is set, the value is negative and equals the unsigned value minus 4096 (2 to the power 12). 0xF60 is 3936 and bit 11 is set (it is greater than 0x800), so 3936 - 4096 = -160. In my code I extract the 12 bits with `& 0xFFF`, test whether 0x800 is set, and if so subtract 4096. That turns 0xFFF into -1. Without this step, -1 would be read as 4095, which is +255.9 degrees C.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
`volatile` tells the compiler that the value can change without the code changing it, because the hardware updates the register. Without it, the compiler may optimise: keep the value in a CPU register instead of reading again, merge several reads into one, or remove a read whose result seems unused. The real hardware read would not happen when needed, and code polling a flag could loop forever on a stale value. `const` promises that the function only reads the register and never writes to it.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
Hardware can change the register at any moment. If I read it once for the flags and again for the temperature, each read may see a different state, and the result would mix two moments in time (for example READY from the old measurement and a temperature from the new one). One read into a local variable gives a consistent snapshot, and I can decode as many fields as needed from it. Reading a hardware register can also have side effects, such as clearing a flag, so reading twice could lose information.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
`0x40021000u` is just a number, an address. The cast `(volatile uint32_t *)` tells the compiler to treat that number as a pointer to a volatile 32-bit unsigned integer. The leading `*` dereferences the pointer, meaning it reads or writes the word at that address. On a microcontroller this address is mapped to a real hardware register (memory-mapped I/O), so the access talks to the peripheral. On a PC with an operating system, that address is not mapped into my process, so the access violates memory protection and the program crashes with a segmentation fault. That is why the tests use an ordinary variable instead.