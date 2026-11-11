# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
`values` has 8 elements, so the valid indexes are 0..7. `values[8]` is the
first bytes right after the array in static memory. Those bytes belong to
whatever the linker placed next, for example the variable `count` or the
`label` pointer. Writing there silently changes another variable. Nothing
crashes at that moment, and the program only misbehaves later, when that
other variable is used, maybe in a completely different function. "It seemed
to work" is dangerous because the bug is still there. It depends on the memory
layout, so it can show up only on another compiler, with other optimization
flags, or on one board out of a hundred, and then it is very hard to trace
back to the real line. AddressSanitizer reported it as
global-buffer-overflow, at the exact line.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
`buf` was a local array, so it lived on the stack. Its lifetime ends when
`log_label` returns, and that stack area is then reused by the next function
that is called. But `label` kept pointing at it, so it became a dangling
pointer: a pointer to memory that is no longer ours. The next call to any
function could overwrite the text "sensor-log/8". Adding `static` moves `buf`
out of the stack into static memory, so its lifetime is the whole run of the
program. Now the pointer stays valid forever and every call returns the same
pointer, exactly as the header requires.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
The sanitizers were silent, because no memory was accessed badly. I found it
because the average test failed: with large readings the result was wrong,
even negative. Reading the code, I saw that the sum of all readings was kept
in an `int16_t`, whose maximum is 32767. To choose the new type I calculated
the worst case: 8 readings of -32768 give -262144, and 8 readings of 32767
give 262136. These need more than 16 bits, but fit easily in `int32_t`
(about +-2.1 billion). So I changed `sum` to `int32_t`. The final average is
always between the smallest and the largest reading, so it fits back into
`int16_t`.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
Microcontrollers have very little RAM. With malloc and free, the heap can get
fragmented over time: there is enough free memory in total, but no single
block that is big enough, so malloc fails after days of running. The time a
malloc call takes is also not predictable, which is bad for real-time code.
If malloc fails on a device with no screen and no user, there is usually no
good way to recover. A fixed static array is reserved once, when the program
is linked. Its size is known in advance and visible in the memory map, it can
never fail at run time, and it never fragments. That is why the log uses
`static int16_t values[LOG_CAPACITY]`.
