# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
`"LED=1"` requires 6 bytes (5 characters plus the terminating `'\0'`), so `strcpy` writes 2 bytes past the end of `buf[4]` on the stack, causing a buffer overflow. This out-of-bounds write can corrupt adjacent local variables, saved registers, or the function's return address, causing the program to crash or jump to an invalid address later when a function returns or reads the corrupted data.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
Returning the total length of `src` allows the caller to detect if truncation occurred because `dst` was too small. The caller checks this by comparing the return value to the buffer capacity, for example: `if (my_strlcpy(buf, src, sizeof(buf)) >= sizeof(buf)) { /* handle truncation */ }`.

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
On platforms where plain `char` is signed (ranging from -128 to 127), characters with values above 127 (such as extended ASCII `0x80` / 128) are treated as negative numbers (-128). Comparing `'A'` (65) with `'\x80'` using signed `char` would evaluate `65 > -128`, incorrectly ordering `'\x80'` before `'A'`, whereas standard lexicographical order requires comparing byte values as `unsigned char` (0 to 255).

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
`sizeof buf` evaluates to `16`, while `my_strlen(buf)` returns `2`. `sizeof` is a compile-time operator that returns the total allocated memory size of the array in bytes, whereas `my_strlen` is a runtime function that counts the number of characters before the first terminating `'\0'` byte.