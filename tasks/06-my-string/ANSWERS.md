# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
"LED=1" needs 6 bytes: 5 characters plus the '\0'. `strcpy` does not know the size of `buf`, it just copies until it finds the '\0', so it writes 'L','E','D','=' into the 4 bytes of `buf` and then '1' and '\0' into the 2 bytes right after it. Those bytes belong to something else: another local variable, a saved register, or the return address of the function on the stack. Nothing fails at that moment. The damage shows up later, when the other code uses its corrupted variable, or when the function returns to a broken return address. So the crash happens in a different place and at a different time from the bug, which makes it very hard to find.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
The number of bytes copied is always known in advance (at most `dst_size - 1`), so returning it would not tell the caller anything new. The length of `src` tells the caller whether the whole string fit: if it is `>= dst_size`, the copy was truncated, and the caller also knows how big a buffer would have been needed. Usage:

    char buf[8];
    if (my_strlcpy(buf, input, sizeof buf) >= sizeof buf) {
        /* truncated: report an error, or reject the command */
    }

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
Whether plain `char` is signed or unsigned depends on the compiler: on x86 with GCC it is signed, on ARM it is usually unsigned. If it is signed, every byte from 0x80 to 0xFF becomes a negative number. For example 'é' in Latin-1 is 0xE9, which as a signed char is -23. Comparing "é" with "a" would then give -23 < 97, so "é" would sort before "a", although its byte value 233 is larger. The same code would also give a different order on an ARM board. Comparing as `unsigned char` gives the same, correct result on every compiler, and that is also what the standard `strcmp` does.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
`sizeof buf` is 16 and `my_strlen(buf)` is 2. `sizeof` is computed by the compiler and gives the size of the whole array in memory, no matter what is stored in it. `my_strlen` runs at runtime and counts the characters before the first '\0': here 'h' and 'i', and then buf[2] is '\0' (the rest of the array is also filled with zeros). The buffer size is how much room there is, and the string length is how much is currently used. Note that `sizeof` only works like this on a real array: on a `char *` parameter it gives the size of the pointer (4 or 8), which is why functions need the size passed separately, like `dst_size`.