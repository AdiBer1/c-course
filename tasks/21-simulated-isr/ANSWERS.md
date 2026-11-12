# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) {}` in the main loop.

Answer:
Without `volatile`, the compiler assumes no external agent can modify the variable behind the main loop's back. In `while (!uart_rx_line_ready()) {}`, the optimizer is allowed to hoist the memory read out of the loop, load the counter into a CPU register once, and spin forever checking that stale register copy even after the ISR increments the variable in RAM.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
On load-store architectures, `count--` executes as three separate instructions: load `count` into a register (say `5`), decrement the register to `4`, and store `4` back to memory. If the ISR fires right after the main loop loads `5` (or computes `4`) and before it stores the result, the ISR loads `5`, increments it to `6`, and writes `6` to memory; when the ISR returns, the main loop finishes its store and overwrites memory with `4`, completely losing the ISR's increment.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
While interrupts are disabled in a critical section or while an ISR is actively running (and blocking lower- or equal-priority interrupts), the CPU cannot respond to new hardware events. If either takes longer than the time it takes for the UART shift register to receive the next byte, the hardware RX buffer overflows (overrun error) and incoming bytes are lost before software can read them.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real UART hardware, the RX-not-empty interrupt request flag remains asserted as long as unread data sits in the data register. If the ISR returns without reading `DR`, the hardware immediately re-triggers the interrupt in an infinite loop, starving the main loop so it can never drain the ring buffer and free up space.