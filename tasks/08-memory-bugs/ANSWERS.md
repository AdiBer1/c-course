# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
log_add wrote one value after the end of the array. This can overwrite other variables in memory, outside the array. [It is dangerous even without a crash because nobody notices the bug, and the wrong data can causeproblems later.]

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
[buf was a local variable on the stack. Its lifetime ended when the function returned, so the returned pointer was a dangling pointer: it pointed to memory that was not valid anymore.] A static variable stays alive even after the function ends, so the pointer stays valid.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
[I found it because the test failed: it expected 30000 but got 8154. int16_t can hold only up to 32767.] I chose int32_t so the sum does not get too big for its type and overflow when calculating the average.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Embedded systems have very little memory. [malloc can fail when there is no memory left, and the device can stop working. A static array has a fixed size that is known before the program runs, so it can never fail.]

