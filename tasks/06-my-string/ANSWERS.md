# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
The string `"LED=1"` requires 6 bytes (5 characters plus the `'\0'` terminator), but `buf` is only allocated for 4 bytes. `strcpy` does not check buffer limits, so it writes past the end of `buf` and overwrites nearby stack memory (like other variables or the return address). This memory corruption might not cause an immediate crash, but instead triggers a crash much later when a completely different function accesses the corrupted memory or tries to use an invalid return address.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
`my_strlcpy` returns `my_strlen(src)` so that the caller can easily detect if truncation occurred. If the returned value is greater than or equal to `dst_size`, the caller knows that the destination buffer was too small to fit the full string.


## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
In C, plain `char` can be signed depending on the platform/compiler. If a byte has a value over 127 (like `0xFF` or extended characters), a signed `char` treats it as a negative number (e.g., `-1`).

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
For `char buf[3] = "hi";`:
- `sizeof buf` is **16**: It measures the total physical capacity of the memory buffer in bytes (evaluated at compile-time).
- `my_strlen(buf)` is **2**: It counts the actual number of string characters currently stored in the buffer up to the `'\0'` byte (evaluated at runtime).

They differ because `sizeof` gives the buffer's allocated capacity, while `my_strlen` measures the length of the string content inside it.
