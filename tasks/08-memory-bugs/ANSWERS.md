# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
log_add wrote one element past the end of values into whatever static memory or padding the linker placed right after the array. "It seemed to work" is dangerous because without a sanitizer, silent memory corruption won't crash immediately and will cause unexplained bugs much later in unrelated code.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
buf was a local array on the stack, so its lifetime ended when the function returned, leaving label as a dangling pointer to reused memory. Making it static puts the buffer in static memory for the entire program run, keeping the pointer valid for every call.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
I found it because the test test_average_of_large_values failed (expected 30000, got 8154) due to 16-bit integer overflow when the sum exceeded 32767. I chose int32_t because it guarantees at least 32 signed bits across all platforms and prevents unsigned conversion issues when dividing by count.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Firmware avoids malloc to prevent heap fragmentation, non-deterministic execution timing, and potential runtime allocation failures on memory-constrained systems. A fixed static array allocates a known amount of RAM at compile time with zero runtime allocation risk.

