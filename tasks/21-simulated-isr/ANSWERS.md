# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
Without `volatile`, the compiler optimizer assumes the counter only changes within the current function flow. It is allowed to load the counter's value into a CPU register once before entering the loop and reuse that cached register value indefinitely. Because the ISR updates the counter asynchronously in RAM, the main loop will never see the update and will get stuck in an infinite loop. `volatile` forces a fresh read directly from RAM on every iteration.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
Read-modify-write operations consist of three assembly steps: loading the variable from RAM into a CPU register, modifying it, and writing it back to RAM. If the main loop reads `count` (e.g., 5) and decrements it in a register, an interrupt firing right before the register is written back will read `count` as 5, increment it to 6, and store 6 in RAM. When control returns to the main loop, it completes its pending write and stores its decremented value (4) to RAM, completely overwriting the ISR's update (6) and losing the increment.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
Both the ISR and critical sections block hardware interrupts to protect shared state. If interrupts remain disabled or an ISR executes for too long, incoming bytes arriving at the UART peripheral cannot trigger an interrupt and will overwrite the hardware receive register before the CPU reads them. This results in UART overrun errors and permanent loss of incoming data bytes.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real hardware, the UART receive interrupt flag stays active until software reads the hardware data register. If the ISR does not read the register when the buffer is full, the hardware interrupt flag is never cleared, causing the hardware to re-trigger the ISR continuously in an infinite loop upon exit and starving the main loop. Reading the register clears the hardware flag and allows the CPU to return to normal execution.
