# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
The string "LED=1" (plus its null-terminator) is 6 bytes long, but the buffer buf only has 4 bytes. strcpy blindly writes past the end of the buffer, corrupting neighboring stack memory. It may crash much later when an unrelated function tries to use the corrupted stack data or return address.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
Returning the full length of src allows the caller to easily check if truncation occurred. The caller compares the return value against the destination buffer size; if it is greater than or equal to the size, they know the string didn't fit.
For example: if (my_strlcpy(buf, s, sizeof buf) >= sizeof buf) 

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
On platforms where char is signed, characters with values > 127 are interpreted as negative numbers. Casting to unsigned char ensures they are treated as positive values (0-255). For example, comparing a high-bit character to a standard ASCII character using signed char could incorrectly judge the high-bit character as smaller (negative).

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
sizeof buf evaluates to the total allocated size of the array in bytes (16), regardless of what is stored inside. my_strlen(buf) counts only the actual characters before the null-terminator \0, which is 2 for "hi"

