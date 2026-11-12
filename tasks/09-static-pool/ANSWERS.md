# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
the storage array is declared `static`, so it lives in static memory, not on the stack or the heap. Its place is reserved at build time (by the linker) and stays for the whole run of the program. This means the linker's RAM report already includes the pool, so I know exactly how much RAM it uses before the device ever runs. There are no surprises at runtime.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
Answer: The user can store any type in a block, so the block must be aligned for the strictest type. Many CPUs require, for example, a 4-byte value to start at an address divisible by 4. The Cortex-M0 does not support unaligned access, so reading or writing a `uint32_t` at an odd address causes a HardFault and the device crashes.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer: After many allocations and frees of different sizes, the free memory is split into small pieces with used blocks between them (fragmentation). The total free space may be large, but no single piece is big enough for the request, so `malloc` fails. In a pool all blocks have the same size, so any free block fits any request. If at least one block is free, the allocation succeeds.


## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:

Answer: I keep a `used` array with one flag per block. If the block is already marked free, it is a double free and I return false. To find the block, I convert the addresses to `uintptr_t` and compute the offset from the start of the pool. If the offset is outside the pool, or `offset % POOL_BLOCK_SIZE` is not 0 (the middle of a block), I return false. Accepting these silently could mark a block as free while it is still in use, so later two parts of the program would get the same block and overwrite each other's data. That kind of bug is very hard to find, so it is better to reject the call immediately.

