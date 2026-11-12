# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:
The storage is a `static` array at file scope, so it lives in static memory (the `.bss` section, because it starts as zeros). It is not on the stack and not on the heap. The memory is reserved at build time: the linker gives it a fixed address, and it exists from start-up for the whole run of the program. That means the 8 * 32 = 256 bytes, plus the flags, already appear in the linker's RAM usage report. If the program does not fit in the chip's RAM, the build fails right away, instead of an allocation failing on the device months later.

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:
The pool does not know what the caller will store in a block: it could be a `uint32_t`, a `double` or a struct containing them. Many types must start at an address that is a multiple of their size, and `max_align_t` has the strictest alignment of all basic types, so a block aligned to it is safe for any type, just like the memory `malloc` returns. A Cortex-M0 cannot do unaligned memory accesses at all: a 32-bit load or store at an odd address raises a HardFault and the firmware crashes. On other chips it may "work" but be slower, or tear the value in half if an interrupt comes in between. In my code `_Alignas(max_align_t)` aligns the start of the array, and because 32 is a multiple of the alignment (checked with `_Static_assert`), every block start is aligned too.

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:
`malloc` hands out blocks of different sizes from one heap. After many allocations and frees in different orders, the free memory ends up split into many small holes between blocks that are still in use. This is called fragmentation. A request for 100 bytes can then fail even when 500 bytes are free in total, because no single hole is 100 bytes long, and on a device that runs for months this gets worse over time. In a pool every block has the same size and every request asks for exactly one block, so any free block fits any request. If `pool_free_count()` is greater than 0, `pool_alloc` always succeeds. There are no holes that are "too small".

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:
First I convert the pointer and the start of the storage to `uintptr_t` and check that the address is inside the pool. Then I compute the offset from the start of the pool. If `offset % POOL_BLOCK_SIZE` is not 0, the pointer points into the middle of a block (for example `p + 1`), and I return false. Otherwise `offset / POOL_BLOCK_SIZE` is the block index. If that block's `in_use` flag is already false, it is a double free, and I return false without changing anything. Accepting these silently would corrupt the bookkeeping: after a double free, the same block could be handed out to two different users who then overwrite each other's data, and `free_count` would become wrong. That kind of corruption shows up much later, somewhere else, and is very hard to trace. Rejecting it immediately turns it into a clear error at the exact place where the bug is.