# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
The struct in this task uses an explicit count variable to distinguish between empty (count == 0) and full (count == size) when head == tail. The other common solution is to waste one slot in the buffer, defining empty as head == tail and full as (head + 1) % size == tail.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
Bitwise AND (index & (size - 1)) executes in a single clock cycle, whereas the modulo operator (%) requires integer division, which takes dozens of clock cycles on microcontrollers without a hardware divider.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The interrupt writes head and count, while the main loop writes tail and count. Because both sides read and modify count, a race condition can occur if the interrupt triggers during a non-atomic read-modify-write of count in the main loop, corrupting its value. To prevent this, operations on count must be atomic or protected inside critical sections (disabling interrupts).

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
Allowing the caller to provide storage decouples the queue implementation from buffer size allocation, permitting multiple instances of arbitrary sizes without dynamic memory allocation (malloc), keeping memory statically allocated at compile time.
