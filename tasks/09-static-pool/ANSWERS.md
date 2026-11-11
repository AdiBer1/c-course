# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The pool's storage lives in static memory (the .bss or .data segment) within `pool.c`, rather than on the stack or heap. It is fully reserved at compile/link time when the binary is built. This means the RAM requirement for the pool is fixed and visible in the linker's RAM report before the program even runs.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
A pool block must be aligned to `_Alignof(max_align_t)` to ensure that any C data structure stored inside the block satisfies the CPU architecture's alignment requirements. On an ARM Cortex-M0 processor, accessing a 32-bit integer at an unaligned address causes an alignment fault / HardFault exception, crashing the system.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
`malloc` can suffer from external memory fragmentation over time, where free memory gets broken into non-contiguous small fragments, preventing allocation of larger contiguous blocks even if total free bytes are sufficient. A pool avoids this issue entirely because all blocks are identical in size, so any available free block can satisfy any allocation request.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
`pool_free` calculates the byte offset of the pointer relative to the pool array, checks if it falls within array boundaries, verifies that the offset is an exact multiple of `POOL_BLOCK_SIZE`, and checks if the block is already marked free. Detecting these errors immediately prevents subtle memory corruption and undefined behavior.