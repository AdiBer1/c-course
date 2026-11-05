# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
If the counter is not declared volatile, the compiler may optimize the while (!uart_rx_line_ready()) loop by reading the variable into a CPU register just once before entering the loop. Since the ISR updates the variable in memory without the compiler knowing that the register cache is stale, the main loop will get stuck in an infinite loop even when lines become ready.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
Modifying a counter is a read-modify-write operation (load from memory to register, decrement/increment, store back). If an interrupt occurs right after the main loop loads the variable into a register but before it writes it back, the ISR will increment the memory value. When the main loop finishes its operation and writes its stale register value back, it overwrites and erases the ISR's update.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
Both must remain extremely short to prevent blocking other high-priority interrupts and missing real-time deadlines. If an ISR or critical section takes too long, subsequent incoming bytes will arrive faster than they can be processed, causing hardware buffer overruns and dropped data.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real microcontroller hardware, the UART receive interrupt flag remains asserted as long as there is unread data in the data register. If the ISR fails to read it, the interrupt will continuously re-trigger in an infinite loop, freezing the entire processor.

