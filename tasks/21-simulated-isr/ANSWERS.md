# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
The compiler only sees the main-loop code. It does not know that an interrupt can jump in and change `lines`. Inside `while (!uart_rx_line_ready()) { }` nothing in the loop writes `lines`, so the optimiser may inline the function, read `lines` once into a CPU register, and then loop on that register forever, or even turn the loop into an endless `while (1)` if the first value was 0. The ISR does update `lines` in memory, but the main loop never looks at memory again, so the program hangs although a line has arrived. `volatile` tells the compiler that the variable can change outside the normal program flow, so every access in the source must really read or write memory, in that order. Then the loop sees the new value as soon as the ISR stores it.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
`count--` is not one operation. On the CPU it is three: load count into a register, subtract 1, store the register back. Say count = 5 and the main loop pops a byte:
1. Main loop loads count: register = 5.
2. A byte arrives, and the interrupt fires right here.
3. The ISR loads count (5), adds 1, stores 6, and returns. Now count = 6 in memory, which is correct.
4. The main loop continues with its OLD register value: 5 - 1 = 4, and stores 4.
Now count = 4, but the buffer really holds 5 bytes: the ISR's increment is lost. The buffer will later "forget" a byte, or think it has room when it is full and overwrite data. With `hal_irq_disable()` before and `hal_irq_enable()` after, the ISR cannot run between steps 1 and 4. The byte waits in the UART until interrupts are enabled again, and then the ISR runs and increments the correct value.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
While interrupts are disabled, and while the ISR is running, no other interrupt can be handled. The UART hardware can usually hold only one byte (or a tiny FIFO) in its data register. At 115200 baud a new byte arrives about every 87 microseconds. If the next byte arrives before the previous one was read, the hardware overwrites it (an overrun error) and the byte is lost for good. Other interrupts (timers, other peripherals) are delayed too, so the system's timing becomes unreliable. That is why the ISR only reads the byte, stores it and updates two counters, and why the line parsing is done in the main loop. The critical section only protects the few instructions that move at most 16 bytes, so any byte that arrives meanwhile simply waits a few microseconds in the data register and is then handled.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real hardware the "receive data ready" interrupt flag stays set until the data register is read: reading it is what clears the flag. If the ISR returned without reading it when the buffer is full, the interrupt would fire again immediately after the ISR returns, again and again forever. The CPU would be stuck in an interrupt storm, and the main loop would never run again to empty the buffer, so the whole device would freeze. Reading the register (and dropping and counting the byte) clears the interrupt, keeps the system running, and lets the main loop catch up. The `dropped` counter shows that data was lost, so the firmware can report it or increase the buffer size.