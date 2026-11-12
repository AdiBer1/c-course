# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
"LED=1" requires 6 bytes (including the null terminator \0), so strcpy writes 2 bytes past the end of buf, causing a stack buffer overflow. This corrupts adjacent stack data such as local variables, saved frame pointers, or return addresses without an immediate fault. The crash happens later when a function tries to read those corrupted values or return to an invalid memory address.
## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
evaluates to 16 because it is a compile-time operator returning the total bytes allocated for the array. my_strlen(buf) returns 2 because it scans the memory at runtime and counts only the characters before the first null terminator (\0).
## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
evaluates to 16 because it is a compile-time operator returning the total bytes allocated for the array. my_strlen(buf) returns 2 because it scans the memory at runtime and counts only the characters before the first null terminator (\0).
## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
evaluates to 16 because it is a compile-time operator returning the total bytes allocated for the array. my_strlen(buf) returns 2 because it scans the memory at runtime and counts only the characters before the first null terminator (\0).
