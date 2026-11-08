# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The pool resides in static memory. This memory is pre-allocated during the program's build process; therefore, the linker knows in advance that the pool requires 256 bytes of RAM, and this is reflected in the program's RAM report.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
A pool block must be properly aligned so that it can safely store any type of data that requires alignment. On an ARM Cortex-M0, accessing a uint32_t at an odd or incorrectly aligned address can cause an alignment fault or other unexpected behavior.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
With malloc, the free memory can become fragmented into many small pieces, so there may not be one large enough continuous area for a new allocation even if there is enough free memory in total. A pool uses equal-size blocks, so any free block can be used for any allocation of the pool's fixed size, avoiding this type of fragmentation.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
pool_free checks that the pointer is exactly the address of one of the pool's blocks and then checks whether that block is currently in use. Detecting invalid pointers and double frees immediately prevents memory corruption and makes programming errors easier to find instead of allowing them to cause problems later.
