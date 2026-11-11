# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
Without `volatile`, the compiler assumes nobody else changes the variable, because the loop body never writes to it. It is allowed to read the counter once, keep it in a CPU register, and turn the loop into an infinite loop on that stale value. The ISR does update the memory, but the loop never reads it again, so the program hangs forever. With `volatile`, the compiler must re-read the variable from memory on every iteration.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
`count--` is not one operation but three: (1) load `count` into a register, (2) decrement the register, (3) store it back to memory. Suppose `count` is 5. The main loop loads 5 into a register. At that moment an interrupt fires and the ISR does `count++`, storing 6. The main loop resumes, decrements its stale copy to 4 and stores 4. The correct result is 5 (one decrement and one increment), but 4 remains: the ISR's increment is lost, and the buffer now believes it holds fewer bytes than it really does.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
While interrupts are disabled, the hardware cannot run the ISR, so an arriving byte has to wait. A UART usually has a single receive register (or a small FIFO), and if a new byte arrives before the previous one is read, it overwrites it or is lost (an overrun). A long critical section or a long ISR delays reading, so at high data rates bytes are lost. A long ISR also blocks other interrupts and hurts the responsiveness of the whole system. That is why the ISR only reads, stores and sets a flag, and heavy work is done in the main loop.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real hardware the interrupt flag is cleared only by reading the data register. If the ISR did not read it, the interrupt would keep firing again and again, the CPU would be stuck inside the ISR and never return to the main loop (which could free up space), and the next byte could not be received. So the ISR always reads the register, and if there is no room it discards the byte and counts it in `dropped`.