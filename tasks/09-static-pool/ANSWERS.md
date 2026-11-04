# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The pool's storage lives in the static memory area (global/static data). The memory is reserved at compile-time, meaning the linker knows its exact RAM footprint beforehand and includes it statically in the memory report.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
Alignment ensures that pointers meet the CPU's hardware addressing requirements. On architectures like ARM Cortex-M0, accessing a 32-bit integer (uint32_t) from an unaligned (odd) memory address triggers a hardware fault (Hard Fault) and crashes the program.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
suffers from fragmentation, where total free memory is plenty, but scattered into small unusable chunks. A memory pool avoids this entirely because all blocks are of a fixed, equal size, meaning any free block can satisfy any allocation request.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
pool_free validates pointers by checking if they fall within the pool bounds, align exactly with a block boundary, and belong to a currently allocated block. Detecting these errors immediately turns silent, catastrophic memory corruption into an explicit, catchable failure.

