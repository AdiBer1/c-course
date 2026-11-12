# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The pool's storage lives in static memory (specifically in the `.bss` section since it is uninitialized static data). This memory is reserved at compile and link time before the program even starts running, meaning the linker's RAM usage report accounts for the entire worst-case memory footprint upfront and guarantees it cannot overflow at runtime.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
Because `pool_alloc` returns a generic `void *` pointer that callers may cast to any data type, each block must satisfy the strictest alignment requirement on the platform (`_Alignof(max_align_t)`). On an ARM Cortex-M0, hardware does not support unaligned 32-bit memory accesses, so attempting to read or write a `uint32_t` at an odd or unaligned address immediately triggers a HardFault exception and crashes the system.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
Over time, allocating and freeing variable-sized objects with `malloc` splits the heap into small, non-contiguous free gaps (external fragmentation), so a request for a larger contiguous block fails even if the sum of free bytes is sufficient. A fixed-size block pool eliminates external fragmentation completely because every block has the exact same size, meaning any free block can immediately satisfy any allocation request.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
`pool_free` checks if the pointer lies within the pool's start and end addresses and verifies that `(addr - base) % POOL_BLOCK_SIZE == 0` to ensure it points to the exact start of a block, then checks the `block_used` array to detect if it is already free. Detecting these errors immediately prevents silent bookkeeping corruption, overlapping allocations, and hard-to-trace memory bugs later in firmware execution.