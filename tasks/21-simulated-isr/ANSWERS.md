# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
The compiler sees that nothing inside the loop changes the counter, so it
may read it once into a register and never read memory again. The loop
then runs forever, even after the ISR set the counter. `volatile` forces a
fresh read from memory every time.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
`count--` is really 3 steps: read, subtract, write. Say count = 5.
1. Main reads 5 into a register.
2. Interrupt: ISR does count++ -> count = 6.
3. Main continues with its old 5, writes 4.
The result is 4 but should be 5, so the ISR's update is lost.
Disabling interrupts around count-- prevents this.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
While interrupts are off or the ISR runs, no new byte can be handled.
The UART holds only one byte in its data register, so if the next byte
arrives before it is read, the old one is overwritten (overrun) and lost.
Long ISRs also delay all other interrupts and the main loop.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real hardware reading the data register clears the interrupt. If the
ISR returns without reading it, the interrupt fires again immediately,
forever, and the CPU is stuck. So we read it, drop the byte, and count it.