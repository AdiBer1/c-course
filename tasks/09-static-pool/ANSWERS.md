# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The pool's storage lives in static memory. It is reserved when the program starts and stays allocated during the entire program. Its size is included in the linker's RAM report.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
We need alignment so different data types can be stored safely. If a `uint32_t` is stored at an odd address, it can cause an alignment problem or an unexpected error.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
This happens because of memory fragmentation. Free memory can be divided into small separate parts, so there may not be one large enough space for a new allocation. In a memory pool, all blocks have the same size, so any free block can be used.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
The function checks if the pointer is inside the pool and points to the start of a block. It also checks if the block is already free. If it is, the function returns false. This helps prevent memory corruption and makes errors easier to find.
