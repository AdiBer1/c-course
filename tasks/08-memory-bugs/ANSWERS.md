# Task 08 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Before your fix, `log_add` wrote `values[8]` when the log was full, and most of the time nothing visibly broke. What memory did it overwrite, and why is "it seemed to work" so dangerous?

Answer:
The `values` array contains 8 elements (indices 0–7), so `values[8]` refers to the memory location immediately following the end of the array. Since `values` is a global static variable, the subsequent bytes likely belong to another static variable, such as `count` or `label`. If the write operation happens to land on `count`, for example, the counter suddenly holds a sensor reading value, causing subsequent writes to occur even further outside the array's bounds. The fact that "it seemed to work" is dangerous because the system does not crash at the moment the bug occurs; instead, the issue manifests later—perhaps in a different part of the code, or only when using a different compiler or running on different hardware. Such a bug might pass all tests on a personal computer yet fail in the field. ASan (AddressSanitizer) detected it because it places "guard zones" around arrays and reports any unauthorized access to them.

## Q2. Explain the bug in `log_label` using the words stack, lifetime and dangling pointer. Why did making the buffer `static` fix it?

Answer:
`buf` was a local variable defined within the function; its lifetime ends when the function returns. Consequently, it resides on the stack, in an area associated with the current invocation of `log_label`. This stack area is deallocated, and subsequent calls to the function will reuse it. However, the code stored the address of `buf` in `label` and returned it, resulting in a "dangling pointer"—it pointed to memory that no longer belonged to the string. A subsequent call to the function could overwrite these bytes, rendering the label's content garbage. Declaring the buffer as `static` changes its lifetime: a static variable is not stored on the stack but rather in the same memory region as global variables, and it persists for the entire duration of the program. Thus, the address remains valid indefinitely, and every call returns the same pointer, as required by the function signature.

## Q3. The average bug with `int16_t sum` is not a memory bug and no sanitizer reported it. How did you find it, and how did you choose the new type?

Answer:
I was unable to find the additional bug on my own; I used Claude to help locate it and understand exactly what the issue was. Here is the explanation:
The log can hold up to 8 readings, each with a value of up to 32,767. This means the sum can reach 262,136—well above `INT16_MAX` (32,767). Consequently, the sum wrapped around, and the resulting average was incorrect. No processor flagged this as an error because there was no invalid memory access or undefined behavior (UB). The addition itself is performed using the `int` type (via integer promotion); the loss of value occurs only when the result is stored back into the `int16_t sum` variable. This is a well-defined conversion (implementation-defined), not undefined behavior. I chose `int32_t` because the maximum possible sum—8 × 32768 = 262,144 (in absolute terms)—fits easily within it. I specified `int32_t` rather than `int` because, on a 16-bit microcontroller, an `int` would also overflow.

## Q4. Why does embedded code avoid `malloc`, and why does this log use a fixed static array instead?

Answer:
In the context of a microcontroller—where the system is not a program that starts and finishes, but rather a device that might operate for years—using `malloc` creates several problems. RAM is very limited; after numerous allocations and deallocations over days or months, the heap becomes fragmented. While there may be sufficient total free memory, there might not be a single contiguous block large enough to satisfy a request—causing `malloc` to suddenly fail, often leaving the device with no effective way to recover. Furthermore, the execution time of `malloc` is unpredictable, which is problematic for real-time code and interrupt handling. Additionally, even a minor memory leak can accumulate over time, eventually causing the device to crash after weeks of operation. A fixed, static array avoids all these issues: its size is known at build time, allowing the linker to immediately report if RAM is insufficient. It cannot fail at runtime, is immune to memory leaks, and remains at a constant memory address throughout the entire execution.
