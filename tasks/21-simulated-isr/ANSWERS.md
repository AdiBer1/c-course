# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
The compiler doesn't know that an interrupt can change the variable. It only sees the main loop, and inside the loop nothing changes the counter. So it is allowed to read the counter one time into a register, and then check only the register again and again. If it was 0, the loop becomes `while (true) { }` and never ends, even after the ISR received a full line. `volatile` tells the compiler "this can change from outside", so it must read it from memory every time.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
Say count = 5.
1. Main loop reads count from memory into a register: 5.
2. Main loop subtracts 1 in the register: 4.
3. Here the interrupt comes. The ISR reads count from memory (still 5), adds 1, writes 6.
4. The ISR ends and the main loop continues from where it stopped. It writes its register, 4, to memory.
Now count = 4, but the right value is 5 (one byte out, one byte in). The ISR's update is lost. The buffer now thinks it has one byte less than it really has. With a critical section the interrupt can't come between step 1 and step 4.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
While interrupts are off, and while the ISR runs, no other interrupt can be handled. The UART has only a very small place for incoming bytes (one data register or a small FIFO). At 115200 baud a new byte comes about every 87 microseconds. If the next byte comes before we read the old one, the old one is overwritten (overrun) and lost. Also other interrupts, like timers, are delayed. So in the critical section I do only the `count--`, and the ISR only reads the byte, stores it and updates counters. The slow work, like copying a line, is done in the main loop with interrupts on.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real hardware, reading the data register is what clears the "byte received" flag. If the ISR returns without reading it, the flag is still set, so the interrupt fires again right away, and again, forever. The CPU is stuck in the ISR and the main loop never runs, so it never empties the buffer either. Reading the register and dropping the byte (and counting it) lets the system continue and the next byte can come in.