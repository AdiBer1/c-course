# Answers - Task 21: Simulated interrupt

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer

If the lines ready counter is not declared `volatile`, the compiler assumes that variables read in a loop cannot be modified asynchronously by external events like interrupts. Consequently, the optimiser is allowed to read the counter's value into a CPU register just once before entering the loop, and then repeatedly check that cached register instead of re-reading from memory. This causes the main loop to become trapped in an infinite loop because it will never observe updates made to the counter by the ISR.


## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer

A high-level decrement operation like `count--` is not atomic; it consists of multiple machine instructions: loading the variable from memory into a register, decrementing the register, and writing the new value back to memory. If an interrupt occurs right after the main loop has loaded the current value into its register but before writing back the decremented result, the ISR will execute, increment the memory variable, and finish. When control returns to the main loop, its register (holding the stale pre-increment value) is written back to memory, overwriting and erasing the update made by the ISR.


## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer

Critical sections disable interrupts, meaning the microcontroller cannot respond to time-sensitive external events while they are active. If critical sections or ISRs take too long to execute, subsequent incoming bytes will arrive faster than the UART hardware FIFO can hold them, leading to buffer overruns, dropped bytes, and unresponsive system performance.


## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer

On microcontroller hardware, the UART receive interrupt request is typically level-triggered or tied directly to the presence of unread data in the receive data register. If the ISR fails to read the register, the interrupt flag remains asserted, causing the CPU to immediately re-enter the ISR the moment it exits, trapping the system in an endless interrupt loop. Reading the data register clears the hardware flag, allowing normal execution to continue even when storage space is unavailable.