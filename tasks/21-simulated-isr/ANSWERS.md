# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
If the counter is not marked `volatile`, the compiler assumes that no external context modifies it within the loop body. The optimizer is allowed to hoist the variable read into a CPU register once before entering the loop, transforming `while (!uart_rx_line_ready())` into an infinite loop `while (true)` if it initially evaluated to 0, completely ignoring updates written by the ISR.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
Decrementing `count--` in the main loop involves three assembly steps: (1) read `count` into a register, (2) decrement the value, (3) write it back. If the ISR triggers right after step 1 and executes `count++` (reading, incrementing, and writing back `count`), control returns to the main loop at step 2. The main loop completes its write-back with the stale decremented value, overwriting and discarding the ISR's increment.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
Critical sections disable interrupts, so long critical sections delay ISR execution and cause incoming UART bytes to be missed or result in hardware overrun errors. Similarly, ISRs run at high priority and block lower-priority interrupts or main-loop execution; keeping both short ensures high responsiveness and prevents hardware buffer overflows.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real UART hardware, receiving a byte sets a hardware status flag that continuously triggers the interrupt line. Reading the hardware data register (DR) is the required hardware side-effect to clear the interrupt flag. Failing to read DR would cause the processor to re-enter the ISR endlessly in an infinite loop upon exit.