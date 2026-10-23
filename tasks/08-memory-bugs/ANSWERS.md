# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:

