# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
Without volatile, the compiler may cache the counter value in a register and loop infinitely without re-reading memory.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
Read-modify-write is non-atomic; if an ISR interrupts mid-operation, it can overwrite or reset the counter update, losing data.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
Short sections prevent blocking high-priority events or dropping incoming bytes due to buffer overflow.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
Reading clears the hardware interrupt flag; otherwise, the hardware would keep firing the interrupt continuously.
