# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
`buf` is allocated only 4 bytes on the stack, but the string `"LED=1"` requires 5 bytes including the null terminator. `strcpy` blindly overflows the buffer, overwriting adjacent stack memory such as local variables or saved return addresses. This often doesn't crash immediately; instead, it corrupts state data that causes a crash much later when an unrelated function attempts to return or use corrupted pointers.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
It returns the full length of `src` so the caller can detect truncation. If the return value is greater than or equal to the destination buffer size (`dst_size`), the caller knows the string was clipped. For example: `if (my_strlcpy(buf, input, sizeof(buf)) >= sizeof(buf)) { /* handle truncation */ }`.

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
On platforms where `char` is signed, characters with values above 127 (like accented or extended ASCII characters) are interpreted as negative numbers. A signed comparison would incorrectly treat a high-value character as smaller than a lower-value positive character. Casting to `unsigned char` treats bytes strictly as positive integers (0–255) for consistent lexicographical order.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
`sizeof buf` evaluates to 16, which is the total static allocation size of the array in bytes at compile time. `my_strlen(buf)` evaluates to 2, which is the runtime length of the string `"hi"` (excluding the null terminator), determined by scanning until the first `'\0'`.
