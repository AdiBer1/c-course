# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
It wrote to the element one past the end of the log's space, where the variable `count` is located. When nobody notices the overwrite, it is dangerous, because later, when that memory location is accessed, nobody will know that the value there is wrong.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
The problem in the function: the variable `label` was supposed to get the value of the log's title. So the first time we entered the function it was `NULL`, we built the value in a local `buf` (on the stack), and we put the address of `buf` into `label`. But after we left the function, the lifetime of `buf` ended and its memory was released. As a result, `label` now points to a broken, irrelevant address (a dangling pointer), and it will point there forever, because its value is no longer `NULL`, so there is no way for the mistake to be fixed.
Making `buf` `static` helped, because then `buf` does not die when the function ends, and `label` always points to something that exists.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
I discovered it simply because a test failed. We investigated why it failed and saw that the value received was different from what was expected. We realized the limits of the `int16_t` type: it couldn't hold a value as large as the one we assigned to it, which caused it to overflow and wrap around to a small value. So we changed it to `int32_t`.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
The risk with `malloc` is that a microcontroller sometimes has too little memory, so it can fail. Also, we don't know in advance how much memory the program will need, and in addition we can forget to free the memory.
That is why we use a fixed array: its size is already known at compile time, so it can't fail and we don't need to remember to free it.
