# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
It wrote outside the array and could damage other memory. Its dangerous because the program might seem to work, but problems can appear later.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
The buffer was on the stack and its lifetime ended when the function finished, but the pointer still pointed to it, its called a dangling pointer and static keeps the buffer alive until the program ends.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
I found the bug because the test gave the wrong result. int16_t was too small for the sum so I changed it to int32_t because it can hold bigger numbers.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Embedded systems have limited memory and malloc can fail and make memory harder to manage. A fixed static array uses a known amount of memory.

