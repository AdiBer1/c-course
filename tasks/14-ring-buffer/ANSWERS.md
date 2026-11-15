# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
Here we just keep a counter of how many bytes are inside. If it's 0 the buffer is empty, if it equals the size it's full, so we never have to guess from head and tail. The other common trick is to always leave one slot unused. Then head == tail can only mean empty, but you lose one byte of space.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
Because it's really cheap. With a size like 16 or 64, the AND gives the same answer as `%`, but it's just one quick instruction. A lot of small chips don't have a divide instruction, so `%` can be slow, and that's a problem inside an interrupt that has to finish fast. The downside is that you can't pick any size you want.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The interrupt writes the byte, moves head and adds 1 to count. The main loop moves tail and takes 1 off count. So both sides change count, and that's the problem. `count--` isn't done in one step: the CPU reads it, changes it and writes it back. If the interrupt jumps in right in the middle, one of the changes gets lost and the count is wrong. To fix it, I'd turn off the interrupt for a moment while the main loop updates count, or use a design without count where each side only touches its own index.

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
So the same code works for any size. One buffer can be 64 bytes for the UART and another 256 bytes for a log, and nobody wastes memory on a size they don't need. The caller also decides where the memory lives, usually a static array, so we know exactly how much RAM is used and don't need malloc at all.
