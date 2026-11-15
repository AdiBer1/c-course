# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
If the counter is not volatile, the compiler may assume that its value cannot change unexpectedly from outside the normal flow of the function. Therefore, in a loop such as `while (!uart_rx_line_ready()) { }`, the optimiser may keep the value in a register or optimise repeated reads instead of checking the actual memory value each time.
The ISR can change the counter asynchronously, so without `volatile` the main loop could keep seeing an old value and potentially wait forever even after a complete line has arrived.


## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
`count--` is a read-modify-write operation, not necessarily one indivisible operation. For example, suppose `count` is 5. The main loop reads 5 and prepares to write 4. Before it writes the result, an interrupt occurs. The ISR reads 5, increments it to 6, and writes 6. Then the main loop continues using its old value and writes 4.
The ISR's update from 5 to 6 has been lost. The correct final value should have been 5, because one byte was removed and one byte was added. Disabling interrupts around the main-loop update prevents the ISR from interrupting this read-modify-write sequence.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
The critical section temporarily disables interrupts, so while it is running the ISR cannot handle newly received bytes. If the critical section were long, more incoming bytes could arrive while interrupts are disabled, increasing the chance that the UART hardware or its receive buffer would overflow.
The ISR must also be short because it runs in interrupt context and can interrupt the main loop. If it spent a long time processing data, other interrupts could be delayed and incoming UART bytes could be missed. The ISR should therefore do only the essential work: read the byte, store it or count it as dropped, update the relevant state, and return.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
The data register must always be read because reading it acknowledges and consumes the received byte from the UART hardware. If the ISR returned without reading the register when the software buffer was full, the received byte could remain pending in the hardware and the receive interrupt could continue to fire. Therefore the ISR first reads the byte and only then decides whether it can store it or must count it as dropped.
