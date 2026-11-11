# Task 08 — Answers

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?
Answer: Writing to `values[8]` overwrote adjacent global memory (specifically the global `count` variable located right after the array). It is dangerous because memory corruption in C often doesn't crash immediately; it can corrupt other variables silently and cause unpredictable bugs hours later or in production boards where debugging is extremely difficult.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?
Answer: The buffer was created as a local variable allocated on the function's stack frame, meaning its lifetime ended as soon as `log_label` returned. Returning a pointer to it resulted in a dangling pointer referencing invalid stack memory. Making the buffer `static` moved its storage to the static data segment, giving it a lifetime that lasts for the entire execution of the program.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?
Answer: It was found through the failing unit tests that checked large values, where the sum exceeded the capacity of `int16_t` and caused an overflow. I chose `int32_t` as the new type because it provides a larger range to safely accumulate sums without overflowing before division.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?
Answer: Embedded systems avoid `malloc` to prevent memory fragmentation, unpredictable allocation failures, and memory leaks over long periods of continuous operation. Using a fixed static array ensures predictable, deterministic memory usage known at compile time, which is crucial for safety and reliability in firmware.