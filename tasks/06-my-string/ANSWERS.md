# Task 06 — Understanding questions

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:

The array has space for only 4 bytes, but the string "LED=1" needs 6 bytes: 5 characters and the terminating '\0'. The strcpy function writes all 6 bytes outside the array's boundaries. This overwrites nearby memory and causes undefined behavior. The program might crash immediately or much later in another function because the overwritten memory may be used only later.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:

The function returns the original length of the source string so the caller can check if the copy was truncated. The number of copied characters alone does not tell us whether the entire string was copied. For example, if the destination buffer is too small, the caller can check:

    if (my_strlcpy(buf, src, sizeof(buf)) >= sizeof(buf)) {
        /* The string was truncated */
    }

This helps prevent unexpected data loss.

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:

Plain char can be signed on some systems, which means bytes with values above 127 may be treated as negative numbers. This can cause an incorrect comparison order. For example, the byte 0x80 should be greater than 0x7F when compared as unsigned char (128 is greater than 127). If char is signed, 0x80 may be treated as -128 and appear smaller. Using unsigned char ensures correct byte comparison.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:

sizeof(buf) returns 16 because the array has space for 16 bytes. my_strlen(buf) returns 2 because the string contains only two characters before the terminating '\0'. The array has extra unused space, so its size is different from the string length.
