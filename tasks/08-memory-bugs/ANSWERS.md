# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
It wrote past the array's end into adjacent static memory or linker padding. Silent corruption is dangerous because it avoids immediate crashes, instead causing unpredictable bugs later in unrelated parts of the program.
## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
Answer:buf was allocated on the stack, meaning its lifetime ended upon return, turning the returned address into a dangling pointer to overwritten memory. Making it static extends its lifetime to the entire program run, keeping the pointer valid.
## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
The unit test test_average_of_large_values failed due to signed 16-bit integer overflow (exceeding 32,767). I chose int32_t to guarantee enough signed bit-width across platforms to hold the sum safely without overflow or unsigned division bugs.
## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Embedded systems avoid malloc to prevent heap fragmentation, unpredictable execution timing, and runtime out-of-memory failures. A fixed static array guarantees a deterministic, compile-time RAM footprint with zero allocation overhead.
