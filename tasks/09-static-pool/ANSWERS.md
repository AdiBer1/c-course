# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The pool storage lives in the static memory area. It is reserved at compile time, which means the linker includes its exact size in the RAM report before the program even runs.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
It must be aligned so any data type can be safely stored inside it. On an ARM Cortex-M0, accessing a 32-bit integer at an unaligned odd address can cause a hardware fault or crash.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
malloc causes memory fragmentation over time. A pool cannot have this problem because all blocks are equal in size, so any free block can satisfy any request.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
It checks if the pointer is within bounds, aligns with block boundaries using modulo arithmetic, and checks if the block is already marked free. Detecting these errors prevents silent data corruption and helps catch bugs early.
