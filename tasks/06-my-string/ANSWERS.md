# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
"LED=1" occupies 6 bytes (5 characters plus the null terminator '\0'), but `buf` holds only 4 characters. Since `strcpy` does not perform bounds checking, it writes 2 bytes beyond the end of the array, overwriting adjacent memory—which may contain other variables, stored values, or a return address. The program will not crash immediately; the crash occurs only when another function reads the corrupted data or attempts to return to a corrupted address—which is why this bug is so difficult to detect.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
`my_strlcpy` returns the length of `src`, so that if the return value is greater than or equal to the size of the destination, the caller can detect that the string has been truncated. This value also indicates how much space would have been required to copy the entire string. Example :

char buf[8];
if (my_strlcpy(buf, s, sizeof buf) >= sizeof buf) {
   
}

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
In a standard `char`, bytes with a value of 128 or greater are considered negative, and are therefore sorted before ASCII characters. The standard `strcmp` performs comparisons as `unsigned char`. For example, consider a comparison between `"\xC3"` (value 195) and `"A"` (value 65): when treated as `unsigned`, the result is `"\xC3" > "A"`; however, when treated as `signed`, the byte `\xC3` becomes -61, resulting in `"\xC3" < "A"`—which is incorrect.
## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
`sizeof buf` is 16, because that is the allocated size of the array in bytes. `my_strlen(buf)` is 2, because the function counts only the characters up to the first `'\0'`. The former represents capacity, while the latter represents the actual length of the content.
