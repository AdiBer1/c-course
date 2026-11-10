# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The storage is static: the `storage` array and the `used` array are file-scope `static` objects, so they live in the static data area (`.bss`, since they are not explicitly initialised), not on the stack and not on the heap. The memory is reserved at link time and exists for the whole life of the program. Therefore the linker counts it in its RAM usage report: 256 bytes for `storage` plus 8 for `used`. The cost is known in advance and does not depend on what happens at run time.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
A block may be used to hold any type, and every type has an alignment requirement (for example a `uint32_t` must sit at an address divisible by 4). `max_align_t` has the strictest requirement, so a block aligned to it is suitable for any type. On a Cortex-M0, an unaligned access to a `uint32_t` causes a HardFault, meaning the CPU crashes, because the hardware does not support such accesses. On other chips it may be slower or read wrong data.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
This is fragmentation. After many allocations and frees of different sizes, the free memory is scattered in small pieces between used blocks. The total number of free bytes may be large, but no single contiguous piece is big enough for the request. In a pool all blocks have the same size, so any free block fits any request. There is no "hole that is too small", so there is no fragmentation, and allocation succeeds as long as at least one block is free.

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
The function converts the address to an integer and computes the offset from the start of the pool. If the offset is outside the pool or not divisible by 32, the pointer is not the start of a block and is rejected. Otherwise it checks `used[index]`: if that is already false, the block is already free, so it is a double free and is rejected. Only after all checks does it release the block. Detecting these errors is better than silently accepting them, because a silent double free or bad pointer corrupts memory: two parts of the code would receive the same block and overwrite each other's data, and the bug would show up long after its cause and be very hard to find. Immediate detection turns it into a clear error instead.