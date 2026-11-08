# Task 21 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. What can go wrong if the "lines ready" counter is not `volatile`? Describe what the optimiser is allowed to do with `while (!uart_rx_line_ready()) { }` in the main loop.

Answer:
Without `volatile`, the compiler assumes that nothing else changes the
variable while my code runs, because in the code of the main loop nobody
writes to it. So with `while (!uart_rx_line_ready()) { }` it is allowed to
read the counter once, keep the value in a CPU register, and never read
it again from memory. Then the loop never sees the change made by the
ISR and runs forever, even after a full line arrived. `volatile` forces
a real read from memory on every check.

## Q2. Your main loop does `count--` and the ISR does `count++`. Explain step by step how an interrupt at the wrong moment loses an update if there is no critical section.

Answer:
`count--` is three steps: read, subtract, write back. Example with
count = 5:
1. The main loop reads count (5) into a register.
2. The interrupt fires now. The ISR reads 5, adds 1, writes 6 (a new
   byte arrived).
3. The main loop continues with its old copy: 5 - 1 = 4, and writes 4.
The correct value is 5 (one byte added, one removed), but count is 4.
The byte from the ISR is "forgotten": the buffer holds 5 bytes while
count says 4. A critical section prevents this, because the ISR cannot
run between the read and the write.

## Q3. Why must the critical section be short, and why must the ISR itself be short? What would happen to incoming bytes otherwise?

Answer:
While interrupts are disabled (critical section), a new byte waits in the
data register. If a second byte arrives before interrupts are enabled
again, it overwrites the first one (an overrun) and a byte is lost. The
same happens when the ISR is long: the next byte can arrive while the
ISR is still running, and other interrupts and the main loop are blocked
for that time too. At a high baud rate a byte arrives every few
hundred microseconds, so both must finish well within that time.

## Q4. Why does the ISR read the data register even when the buffer is full and the byte will be dropped?

Answer:
On real hardware the RX interrupt flag stays set until the data
register is read. If the ISR returned without reading it, the interrupt
would fire again immediately, over and over (an interrupt storm), and
the main loop would never run again. So the ISR always reads the byte
first. If there is no room, the byte is dropped and counted in
`dropped`, which is a controlled loss instead of a hang. (The simulator
also counts ISR runs without a read as `unread`.)
