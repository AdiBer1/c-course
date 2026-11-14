# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
If lines_ready is not volatile, the compiler may assume that its value does not change unexpectedly. It may optimize repeated reads and keep an old value instead of checking memory again. As a result, the main loop might never notice that the ISR has received a complete line and may wait forever.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
The problem happens because count-- and count++ are read-modify-write operations. First, the main loop reads the value of count, for example 5. Before it writes the new value, an interrupt occurs. The ISR reads 5, increments it to 6, and writes 6. When the main loop resumes, it uses its old value and writes 4. The ISR's update is lost, and the correct value should have been 5.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
A critical section must be short because interrupts are disabled during it. If it takes too long, incoming UART bytes may not be handled on time. The ISR must also be short because it blocks other work and may delay handling additional interrupts. If bytes arrive faster than they can be processed, the hardware or receive buffer may overflow and bytes may be lost.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
The ISR must always read the data register because reading it clears the receive condition on the hardware. If the ISR does not read it, the UART interrupt may keep firing because the received byte is still waiting in the register. This can cause repeated interrupts and prevent the system from handling other work correctly. Therefore, even if the buffer is full, the ISR reads the byte and increments the dropped counter.

