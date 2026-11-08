# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
The length of "LED=1" is 5, plus `'\0'` that makes 6 characters. `buf` has room for only 4 characters, but the compiler does not know in advance the length of the parameters, so the code compiles. But when the copy is done, characters overflow past `buf`, and data that sits after it in memory can be overwritten. A crash will happen later, when the overwritten data is used, if it is no longer valid.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
So that we can know whether all the characters of the source fit into the destination, because if the source is longer than the destination, some of the characters will not fit.

Example:
```c
char buf[8];
size_t len = my_strlcpy(buf, src, sizeof buf);
if (len >= sizeof buf) {
    /* truncated: we needed len + 1 bytes */
}
```

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
It is not defined whether characters are signed or not: on some computers they are and on others they aren't. So when we handle characters up to the value 127, everything works as usual. But for characters with a higher value, the MSB, which is 1, is interpreted as a sign bit, so they are seen as negative values and are treated as if they were smaller than the values of the first characters.
For example, the byte value `0x80` equals 128, and the character `'A'` equals 65, so `'A'` should come first, because that is the correct order. With signed `char`, the first character is computed as -128 and comes before `'A'`, which is wrong.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
`sizeof buf` is the total size of `buf`: how much it can hold and how much space is reserved for it in memory. The string length is how long the string stored in `buf` actually is.
The value of `sizeof buf` is 16, and the value of `my_strlen(buf)` is 2. 
