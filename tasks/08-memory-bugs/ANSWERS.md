# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
`values` has 8 elements (indexes 0..7), so `values[8]` is the 2 bytes right after the array in static memory. The linker usually puts other global variables there. In this file that can be `count` or `label`, or padding, or a variable from a completely different module. So the write silently changed some other variable instead of failing. "It seemed to work" is dangerous because the program does not crash where the bug is. It corrupts data that is used later, somewhere else, and the symptom depends on the memory layout: it can disappear after an unrelated change, or appear on only one build or one board. Only a tool like AddressSanitizer reported it at the exact line (`global-buffer-overflow`).

values[8] is one element past the array, in static memory, so it overwrote whatever variable the linker placed right after it (here likely `count` or `label`). Nothing checks array bounds in C, so the program continues with corrupted state and fails later somewhere unrelated, or only on another compiler or board where the layout differs. AddressSanitizer makes the bad write visible at the exact line.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
`char buf[16]` was a local variable, so it lived on the stack, and its lifetime ended when `log_label` returned. The function stored its address in the static `label` and returned it, so after the return `label` was a dangling pointer: it pointed to stack memory that no longer belonged to anyone. The next function call reuses that stack area and overwrites the text. It sometimes "worked" only because nothing had overwritten that memory yet. Making `buf` `static` moves it out of the stack into static memory, where its lifetime is the whole run of the program, so the pointer stays valid and every call returns the same pointer, as the header requires.

`buf` was a local array on the stack; its lifetime ends when log_label returns, but its address was saved in `label` and handed out. That pointer then pointed to stack memory that the next function calls reuse: a dangling pointer. A `static` array has static storage duration: it exists for the whole run, so the pointer stays valid.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
It was found by a failing test that averages large readings: the result was wrong (even with the wrong sign), although no memory was accessed badly. Reading the code, I asked what the largest possible sum is: 8 readings of at most 32767 each give 262,136, and the smallest is 8 * -32768 = -262,144. That does not fit in `int16_t` (max 32,767), so `sum += values[i]` wrapped around when the result was stored back into the 16-bit variable. I chose `int32_t`, which holds about ±2.1 billion, far more than any possible sum of 8 readings, and is a natural size on a 32-bit microcontroller. The division then gives a result in the `int16_t` range, so the final cast is safe.

A test with three readings of 30000 failed: the sum 90000 does not fit in 16 bits and wrapped. The worst case is LOG_CAPACITY readings of 32767 (or -32768), about 262,000 in magnitude, so int32_t is enough with a large margin.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:

With a few KB of RAM, heap fragmentation can make an allocation fail after days of running, at a moment that cannot be tested in advance; malloc also has unpredictable timing and needs extra RAM for bookkeeping. A static array has a size known at link time: if it fits, it always fits, and the linker map shows exactly how much RAM is used.
