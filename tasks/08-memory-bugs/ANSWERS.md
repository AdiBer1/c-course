# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
It overwrote memory outside the array, which could belong to another variable or data. This is dangerous because memory corruption may not cause an immediate crash, so the program can appear to work while producing unexpected results.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
The buffer was stored on the stack, so its lifetime ended when the function returned. The label pointer became a dangling pointer. Making the buffer static keeps it alive for the entire program.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
I found it by checking the maximum possible sum. Eight int16_t values can exceed the range of int16_t, so I changed the sum to int32_t.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Embedded code often avoids malloc because dynamic memory allocation can cause fragmentation and unpredictable memory usage. A fixed static array provides predictable memory usage and does not require dynamic allocation.
