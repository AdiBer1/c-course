# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
If only head and tail exist, head == tail looks the same when the buffer is
empty and when it is full. This struct solves it with a separate `count`
field: count == 0 means empty and count == size means full, so all `size`
bytes can be used. The other common solution is to always leave one slot
unused: the buffer is "full" when advancing head would reach tail. That
needs no count, but it wastes one byte.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
`index & (size - 1)` is a single, very fast AND instruction when size is a
power of two (for example size 64: the mask is 63 = 0b111111, which keeps
only the low 6 bits, so the index wraps automatically). A `%` needs a
division, and many small MCUs (like Cortex-M0) have no hardware divider,
so the division is a slow software routine. A comparison with `if` works,
but adds a branch. The AND is fast and takes the same time every call.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The interrupt (producer) writes buf[head], head and count (count++). The
main loop (consumer) writes tail and count (count--). So `count` is written
by both sides. `count++` and `count--` are read-modify-write operations,
not atomic: if the interrupt fires between the main loop's read of count and
its write back, one update is lost and count becomes wrong (the buffer
may look full, empty or lose bytes). To fix it, I would disable interrupts
around the update of count in the main loop (a short critical section). A
second option is a design without a shared count: only the interrupt
writes head, only the main loop writes tail, and the number of bytes is
computed from them (this needs the "one slot unused" solution or
free-running indices).

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
With caller-provided storage, one implementation works for buffers of any
size, and each user chooses the size it needs. The memory is allocated
statically (no malloc, which is avoided in firmware because of
fragmentation and unpredictable failures), so the memory use is known at
compile time. The caller can also decide where the array lives, for example
in a specific RAM region or in memory that a DMA controller can reach.
