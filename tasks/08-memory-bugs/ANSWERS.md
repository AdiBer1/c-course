# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
Writing to values[8] goes past the end of the array and overwrites whatever happens to sit in memory right after it. In my run, the sanitizer showed that the global variable count is located close after values (48 bytes later in the sanitizer build; without the sanitizer it could sit directly after it). Without sanitizers, this would silently corrupt another variable. That "it seems to work" behavior is dangerous because there's no immediate crash—the program keeps running, and the actual bug might only show up much later, in a completely different part of the code, or only on certain devices.


## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
buf was a local variable allocated on the stack, meaning its lifetime ended as soon as the function returned. Returning a pointer to it left label as a dangling pointer pointing to reclaimed memory. Any subsequent function calls reuse that same stack space and overwrite the text. Declaring buf as static moves it into static storage that persists for the entire run of the program, so the pointer remains valid.


## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
No sanitizer caught this one—I found it because a unit test failed (Expected 30000 Was 8154). In the worst case, we're summing 8 values of around 32,767, which totals roughly 262,000. That overflows int16_t, so I switched the sum variable to int32_t, which holds that total easily.


## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Microcontrollers have very limited RAM. Using malloc can lead to heap fragmentation or running out of memory mid-execution, causing an allocation to fail unexpectedly. Its execution time is also non-deterministic. With a fixed-size static array, the exact memory usage is known at compile time, so you know upfront that it fits and there are no runtime allocation failures.


