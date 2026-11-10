# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
the next address of memory, it just after the memory allocated for the values aray, "48 bytes before count", this couses a crash because the code tried to assign a value into a memory cell which was not allocated for the program/

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
Every function gets a stack, there are saved the local variables and so more data (like return function, and so on), the second that the function is finished (after the return), all this stack is deleted, so a variable that was localy in the function is deleted the second the function finished, and tring to point to it again and assign\use will couse a memory error, because its garabage, making it 'static' make it to save the variable not localy in the stack - but in the main stack\heap of the program, so it live the whole program/
## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
I found it quick, because we talked alot in the lessons about this problem and also in the other tasks we handled this, i chose int32_t because it provides a much larger range than int16_t, allowing the sum of all 8 possible int16_t values without overflowing.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Embedded systems often avoid malloc because dynamic memory allocation can cause heap fragmentation, unpredictable allocation times, and allocation failures at runtime. A fixed static array provides predictable memory usage, avoids runtime allocation, and makes memory management more reliable in resource-constrained environments. All the reasons why we like compilation errors :)

