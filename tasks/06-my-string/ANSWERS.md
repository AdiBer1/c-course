# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
The buffer has space for only 4 bytes, but the string needs 6 bytes, including the '\0'. strcpy writes beyond the buffer and overwrites other memory. This can damage other variables and cause a crash later in a different function.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
It returns the original length of the source string so the caller can know if the string was truncated. For example, if the returned length is greater than or equal to the destination size, the buffer was too small.

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
Because plain char can be signed, so characters with values above 127 may be treated as negative numbers. Using unsigned char makes sure all character values are compared correctly as positive values. For example, a character with value 200 could be treated as -56 when using signed char.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
sizeof buf is 16 because it returns the total size of the array in bytes. my_strlen(buf) is 2 because it counts only the characters before the '\0'. The array has extra unused space.
