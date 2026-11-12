# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
The temperature is 12 bits, and the top bit (bit 11) is the sign bit. If it's 0, the value is positive as is (0 to 2047). If it's 1, the value is really negative, and you get it by subtracting 4096 (that's 2 to the power of 12). 0xF60 is 3936, the top bit is on, so the real value is 3936 - 4096 = -160. In my code I take the 12 bits out into an int32_t, and if the value is 2048 or more, I subtract 4096. That's sign extension: it turns a 12-bit negative number into a normal negative number.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
`volatile` tells the compiler that the value can change by itself, from the hardware, without my code touching it. So the compiler has to actually read it from memory every time I ask. Without `volatile`, the compiler can assume the value never changes. It might read it once and reuse the old value, or even remove the read completely. Then my code would miss new data from the sensor. `const` means my function only reads the register and never writes to it.
## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
The sensor can update the register at any moment. If I read it once for the temperature and again for the error flag, the sensor might change it in between. Then I'd get the temperature from one moment and the error flag from another, a mix that never really happened. So I read it once into a normal variable and decode everything from that copy. That way all the fields come from the same moment.
## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
Piece by piece:
- `0x40021000u` is just a number, a memory address.
- `(volatile uint32_t *)` turns that number into a pointer to a 32-bit value that can change on its own.
- The `*` at the start reads the value stored at that address.
On a microcontroller this works because that address is connected straight to a hardware register (memory-mapped I/O), so reading it reads the hardware. On a PC, the operating system gives every program its own memory, and that address doesn't belong to my program. So trying to read it crashes with a segmentation fault.
