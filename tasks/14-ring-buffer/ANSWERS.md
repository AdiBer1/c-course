# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
This struct solves the ambiguity by maintaining an explicit count variable to track the number of stored elements (0 means empty, capacity means full). The other common solution is to waste one slot in the buffer, leaving a gap between head and tail so that head == tail always means empty, and (head + 1) % size == tail means full.
## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
Integer division and modulo (%) are computationally expensive and often lack dedicated hardware instructions on low-end microcontrollers, taking many CPU cycles. A bitwise AND (&) is a single-cycle instruction with no branching overhead, making wrap-around calculations fast and deterministic.
## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The ISR writes to head and count, while the main loop writes to tail and count. Because both contexts perform a non-atomic read-modify-write on count, an interrupt occurring mid-update in the main loop can corrupt count (a race condition). To prevent this, either briefly disable interrupts around the count update in the main loop (critical section), or eliminate count entirely using the one-empty-slot method so each context only writes to its own index.
## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
Passing the buffer externally allows a single ring buffer implementation to be reused across multiple instances with different capacities and data sizes. It also avoids dynamic heap allocation (malloc), letting the caller statically allocate exact memory budgets—which is critical in safety-critical and memory-constrained embedded systems.
