# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The pool's storage lives in static memory. It is reserved when the program starts and stays allocated for the entire program. The linker includes this memory in the program's RAM usage.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
Alignment makes sure that data is stored at an address suitable for its type. Without proper alignment, accessing a uint32_t at an odd address on an ARM Cortex-M0 may cause an unaligned access fault or unexpected behavior.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
This can happen because of memory fragmentation. Free memory may be split into small separate pieces, so there is no large enough continuous block. A pool uses fixed-size blocks, so it does not suffer from this type of external fragmentation.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
The function checks that the pointer is inside the pool and that its offset is a multiple of the block size. It also checks that the block is currently in use. This detects invalid pointers and double frees. Detecting these errors helps prevent memory corruption and bugs.

