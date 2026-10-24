# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:

values[8] is one element past the array, in static memory, so it overwrote whatever variable the linker placed right after it (here likely `count` or `label`). Nothing checks array bounds in C, so the program continues with corrupted state and fails later somewhere unrelated, or only on another compiler or board where the layout differs. AddressSanitizer makes the bad write visible at the exact line.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:

`buf` was a local array on the stack; its lifetime ends when log_label returns, but its address was saved in `label` and handed out. That pointer then pointed to stack memory that the next function calls reuse: a dangling pointer. A `static` array has static storage duration: it exists for the whole run, so the pointer stays valid.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:

A test with three readings of 30000 failed: the sum 90000 does not fit in 16 bits and wrapped. The worst case is LOG_CAPACITY readings of 32767 (or -32768), about 262,000 in magnitude, so int32_t is enough with a large margin.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:

With a few KB of RAM, heap fragmentation can make an allocation fail after days of running, at a moment that cannot be tested in advance; malloc also has unpredictable timing and needs extra RAM for bookkeeping. A static array has a size known at link time: if it fits, it always fits, and the linker map shows exactly how much RAM is used.
