# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
Writing to `values[8]` overwrote the static memory immediately following the `values` array. It is dangerous because silent memory corruption does not crash the program right away; instead, it unpredictably alters adjacent variables, causing hard-to-trace bugs or crashes much later in unrelated functions.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
The array `buf` was allocated on the stack, so its lifetime ended as soon as `log_label()` returned. Assigning its address to `label` created a dangling pointer to invalid stack memory. Making `buf` static extended its lifetime to the entire execution of the program, ensuring the memory remains valid.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
The unit test `test_average_of_large_values` failed because summing multiple `int16_t` values caused integer overflow past 32,767. I found it by inspecting the failed test result and chose `int32_t` for `sum` because 32 bits easily accommodates the maximum possible sum of `int16_t` values without overflowing.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Embedded systems avoid `malloc` to prevent memory fragmentation, non-deterministic allocation delays, and heap exhaustion on memory-constrained hardware. Using a fixed static array guarantees predictable memory allocation entirely determined at compile time.

