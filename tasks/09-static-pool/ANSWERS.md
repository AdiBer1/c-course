# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer: My storage array is `static`, so it's not on the stack and not on the heap. The memory is saved for it when the program is built, and it stays there the whole time the program runs. So the linker's RAM report already counts it, and I know before running the device exactly how much RAM the pool takes. Nothing changes while it runs.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer: Someone can put any type into a block, so the block has to start at an address that works for every type. Some processors need, for example, a 4-byte number to start at an address that divides by 4. The Cortex-M0 can't read or write a `uint32_t` at an odd address. If you try, you get a HardFault and the device crashes.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer: After a lot of mallocs and frees of different sizes, the free memory gets broken into small pieces with used memory in between. This is called fragmentation. There can be enough free bytes in total, but no single piece is big enough, so `malloc` fails. In my pool all the blocks are the same size, so any free block fits any request. If even one block is free, the allocation works.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer: I have a `used` array that remembers which blocks are taken. If someone frees a block that's already free, that's a double free, so I return false. To find the block, I turn the addresses into numbers (`uintptr_t`) and check how far the pointer is from the start of the pool. If it's outside the pool, or the distance doesn't divide by 32 (so it's in the middle of a block), I return false. If I just accepted these, a block could be marked free while someone is still using it. Then two parts of the program could get the same block and overwrite each other's data. That kind of bug is really hard to find, so it's better to catch it right away.