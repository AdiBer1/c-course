# Task 11 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. The temperature field 0xF60 means -160, not 3936. Explain two's complement sign extension for a 12-bit field and how your code does it.

Answer:
In 12-bit two's complement, bit 11 (the top bit of the field) is the sign bit and has the weight -2048 instead of +2048. Values 0x000..0x7FF are 0..2047, and 0x800..0xFFF are -2048..-1. When the field is extracted into a 32-bit variable, the upper 20 bits are 0, so a negative value looks like a large positive number: 0xF60 reads as 3936. Sign extension means copying the sign bit into all the upper bits so the 32-bit value means the same number. My code does it with arithmetic: if bit 11 is set (`raw & 0x800`), the real value is `raw - 4096` (4096 = 2^12). For 0xF60: 3936 - 4096 = -160, which is -10.0 degrees C. It works because a 12-bit pattern with the top bit set represents exactly `raw - 2^12`.

## Q2. Why does `status_read` take a `const volatile uint32_t *`? What could the compiler do with the register access if `volatile` were missing?

Answer:
`volatile` tells the compiler that the value at this address can change at any time by something outside the program (here, the sensor hardware), so every access written in the code must really happen, in that order. Without it, the compiler assumes memory only changes when the program writes it. In a polling loop like `while (!(*reg & READY))` it may read the register once, keep the value in a CPU register and loop forever, or skip a read whose result it thinks it already knows. `const` means our code only reads the register and never writes it, so a mistaken write is a compile error. Together they describe a read-only hardware register exactly.

## Q3. Why must the register be read only once in `status_read`, even though decoding needs several fields?

Answer:
The hardware updates the register whenever it wants, also between two instructions of our code. If each field were read with a separate `*reg`, the fields could come from different moments: for example, READY from before a new measurement and FIFO_LEVEL or TEMPERATURE from after it, giving a combination that never existed in the chip. On some chips reading a status register also has side effects, like clearing a flag, so extra reads can lose information too. Reading it once into a local `snapshot` and decoding all fields from that copy guarantees a consistent picture of one single moment.

## Q4. What does `*(volatile uint32_t *)0x40021000u` mean, piece by piece? Why does this work on a microcontroller but crash on your PC?

Answer:
`0x40021000u` is just an unsigned number: the address where the chip's datasheet says the register is. `(volatile uint32_t *)` casts that number into a pointer to a 32-bit value that can change outside the program's control. The `*` in front dereferences it, so the whole expression is "the 32-bit value at address 0x40021000", read fresh every time. On a microcontroller this works because there is no virtual memory: that address is wired directly to the peripheral's register (memory-mapped I/O). On a PC every program runs in its own virtual address space managed by the operating system, and that address is not mapped for our process, so accessing it causes a segmentation fault, and even if it were mapped it would be ordinary RAM, not a sensor.