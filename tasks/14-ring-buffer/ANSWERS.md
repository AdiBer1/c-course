# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
This struct solves the ambiguity by keeping an explicit `count` field, where `count == 0` means empty and `count == size` means full. The other common solution is wasting one byte/slot in the buffer capacity (never allowing it to fill completely), so that `head == tail` strictly means empty, and `(head + 1) % size == tail` means full.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
The modulo operator `%` requires integer division, which is slow, cycle-intensive, and generates larger code on microcontrollers (many of which lack hardware division units). Bitwise AND `&` executes in a single clock cycle on a power-of-two size, making index wrap-around vastly more efficient.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The receive interrupt (producer) writes to `head` and increments `count`, while the main loop (consumer) writes to `tail` and decrements `count`. This is dangerous because `count` is modified by both sides via non-atomic read-modify-write operations, creating race conditions if an interrupt occurs mid-update. To fix this, one should either temporarily disable interrupts during buffer updates or remove `count` entirely and use atomic/`volatile` `head` and `tail` pointers for lock-free single-producer single-consumer operation.

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
Providing the storage array separately decouples the ring buffer logic from memory allocation. This provides full flexibility for the caller to decide the buffer size and allocation strategy (static, stack, or dynamic memory) according to their embedded system's strict RAM constraints, while keeping the struct definition small and reusable.

