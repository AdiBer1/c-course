# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
It writes "LED=1" and the '\0' into a buffer that has space for only 4 bytes. This overwrites memory outside the buffer. It can change other variables or pointers, causing a crash later in a different function.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
It returns the full length of the source string, even if not all characters were copied. This helps us detect truncation. For example, if the returned length is greater than or equal to the destination size, we know the string was truncated.

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
Plain char can be signed, so characters above 127 may become negative. Using unsigned char makes sure we compare their actual byte values. For example, 0x80 should be greater than 0x7F, but signed char may give the wrong order.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
sizeof buf is 16 because it returns the total array size. my_strlen(buf) is 2 because the string contains only 'h' and 'i' before the '\0'.
