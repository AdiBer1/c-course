# Task 09 — Answers

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?
Answer: The pool's storage lives in the static data segment. It is reserved at compile time, which means the linker's RAM report shows a fixed, deterministic memory consumption known before the microcontroller even boots up.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?
Answer: Alignment ensures that any data type can be safely stored in the block without violating hardware requirements. On an ARM Cortex-M0, accessing a multi-byte type like `uint32_t` from an unaligned (e.g., odd) address can trigger a hard fault or alignment exception, crashing the firmware.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.
Answer: `malloc` suffers from memory fragmentation, where free memory is broken into small, non-contiguous chunks that cannot satisfy a large allocation request. A pool of fixed, equal-size blocks completely eliminates fragmentation because any free block can satisfy any request.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?
Answer: A double free is detected by checking if the block's tracking status is already marked as free. A non-start pointer is detected by checking if the address offset from the pool base is within bounds and evenly divisible by the block size. Detecting these errors immediately prevents silent memory corruption, which is notoriously difficult to debug in embedded systems.