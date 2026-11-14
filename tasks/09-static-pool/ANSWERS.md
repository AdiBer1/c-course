# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The pool's storage is a static array, so it lives in static memory (the .bss section, since it is zero-initialised), not on the stack or the heap. The space is reserved at link time, before the program even starts, and it exists for the whole run. Because of that, the linker's RAM report already includes the full 8 × 32 = 256 bytes plus the bookkeeping, so the real worst-case memory use is known and visible at build time, instead of being hidden in a heap that may run out at runtime.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
The caller may store any type in a block (a double, a uint64_t, a struct), and _Alignof(max_align_t) is the strictest alignment any basic type needs, so a block aligned to it is safe for every type, just like malloc guarantees. On an ARM Cortex-M0 the CPU does not support unaligned access: a 32-bit load or store (LDR/STR) to an address that is not a multiple of 4 raises a HardFault, and the device crashes or resets. On other cores unaligned access may "work" but be slower, so it is a hidden bug either way.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
With malloc, blocks of different sizes are allocated and freed in random order, so free memory gets split into many small holes between used blocks (external fragmentation). The total free bytes can be large, but if no single hole is big enough for the request, malloc fails. In a pool every block has the same size and every request asks for exactly that size, so any free block fits any request. There are no "too small" holes: if the free count is above zero, the allocation succeeds.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
pool_free converts the pointer to an integer and computes its offset from the start of the pool. If the offset is outside the pool's range it returns false; if the offset is not a multiple of POOL_BLOCK_SIZE, the pointer is in the middle of a block and it returns false. Otherwise offset / block size gives the block index, and if in_use[index] is already false it is a double free, so it returns false. Detecting these is better than accepting them silently because a silent double free would mark a block free twice, so it could be handed out to two users at once, corrupting each other's data. That kind of bug shows up much later and is very hard to trace, while an immediate false points straight at the faulty call.
