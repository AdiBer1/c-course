# Task 14 — Answers

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer: 
This struct solves ambiguity by maintaining an explicit `count` variable that tracks the exact number of elements stored. The other common solution is to leave one slot in the buffer permanently empty so that `head == tail` strictly means empty while `(head + 1) % size == tail` means full.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer: 
The modulo operator (`%`) often compiles into a costly division or modulus instruction, which is slow on microcontrollers lacking hardware division support. A bitwise AND (`&`) with a power-of-two mask executes in a single CPU cycle, vastly improving performance.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer: 
The interrupt handler writes to `head` and increments `count`, while the main loop reads and writes to `tail` and decrements `count`. This creates a race condition because operations like `count++` or `count--` are not atomic on many 8-bit or 32-bit architectures, meaning an interrupt could corrupt `count` mid-update. To fix this, you should disable interrupts briefly around count modifications or use atomics/volatile structures.

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer: 
By letting the caller provide the storage, a single generic `ring_buffer_t` implementation can manage buffers of any size and type without dynamic memory allocation (`malloc`), allowing complete flexibility for static allocation across different drivers.