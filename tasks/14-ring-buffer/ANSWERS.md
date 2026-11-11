# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
Here we keep a `count` field, so we don't need to guess. If count is 0 the buffer is empty, if count equals size it is full. The other common way is to never fill the last slot: the buffer is "full" when head + 1 == tail (with wrap-around). Then head == tail always means empty. The cost is that you lose one byte, so a buffer of 64 can only hold 63.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
Because `&` is one very fast instruction. `%` needs a division, and division is slow on small microcontrollers, some of them don't even have a hardware divider so it is done in software. With a size like 64, `index & 63` gives the same result as `index % 64`. It also has no `if`, so there is no branch. That matters when the code runs inside an interrupt many times.

## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The interrupt (push) writes `head` and `count`. The main loop (pop) writes `tail` and `count`. The problem is that both sides write `count`. `count--` is not one step, it is read, subtract, write. If the interrupt comes in the middle and does `count++`, the main loop then writes back its old value and the interrupt's change is lost. Then the count is wrong and we can lose or repeat bytes. To fix it I would turn off interrupts for a short moment around the change of `count` in the main loop (a critical section). Another option is to remove `count` and use only head and tail, so each side writes only its own index, and mark them `volatile`.

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
So the same code works for any size. The UART can have a buffer of 64 and the logger a buffer of 512, with one implementation. The caller can make the array `static`, so there is no malloc and we know at compile time how much RAM is used. The caller also decides where the memory is, for example a special RAM area for DMA.