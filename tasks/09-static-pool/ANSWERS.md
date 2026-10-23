# Task 09 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Where in memory does your pool's storage live (stack, static, heap)? When is that memory reserved, and what does that mean for the linker's RAM report?

Answer:

## Q2. Why must a pool block be aligned to `_Alignof(max_align_t)`? What could happen on an ARM Cortex-M0 if someone stored a `uint32_t` at an odd address?

Answer:

## Q3. With `malloc`, a long-running device can fail to allocate even though enough bytes are free in total. Explain why, and why a pool of equal-size blocks cannot have this problem.

Answer:

## Q4. How does your `pool_free` detect a double free and a pointer that is not the start of a block? Why is it better to detect these than to silently accept them?

Answer:

