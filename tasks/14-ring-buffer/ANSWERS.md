# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
The count field stores how many bytes are currently in the buffer.
If count is 0, the buffer is empty. If count equals size, the buffer is full.
Another common solution is to leave one slot unused to distinguish full from empty.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
When the size is a power of two, index & (size - 1) can replace modulo.
This is often faster on microcontrollers because bitwise operations are efficient.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The interrupt writes to head and count, while the main loop writes to tail and count.
This is dangerous because both sides modify count, which can cause a race condition.
I would protect count updates using a critical section or use a design with separate atomic indices.

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
The caller provides the storage array so it can control the buffer size and memory location.
This avoids dynamic memory allocation and is useful in embedded systems with limited memory.
