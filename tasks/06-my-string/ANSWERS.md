# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
"LED=1" needs 6 bytes: 5 characters plus the terminating '\0'. `buf` has only
4 bytes. `strcpy` does not know the size of `buf`; it just copies until it
finds '\0' in the source. So it writes 'L','E','D','=' into buf, and then
writes '1' and '\0' into the 2 bytes right after buf. Those bytes belong to
something else: another variable, saved registers, or the function's return
address on the stack. Nothing stops the program at that moment. The damage
is noticed only later, when that other variable is used or when the function
returns to a corrupted address. That is why the crash appears in a different
place from the real bug, which makes it very hard to find.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
The number of bytes copied is not useful: the caller already knows it is at
most dst_size - 1. The length of src tells the caller whether everything fit.
If the return value is >= dst_size, the string was cut. It also tells how big
the buffer needs to be (return value + 1 for the '\0').
Example:

    char buf[8];
    if (my_strlcpy(buf, name, sizeof buf) >= sizeof buf) {
        /* name was truncated: report an error or use a bigger buffer */
    }

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
Whether plain `char` is signed or unsigned depends on the compiler and the
CPU (signed on x86, usually unsigned on ARM). If it is signed, every byte from
0x80 to 0xFF becomes a negative number. Then the order of the strings would
depend on the platform, and it would be wrong.
Example: the byte 0xE9 (for example 'é' in Latin-1) is 233 as unsigned char,
but -23 as signed char. Comparing "\xE9" with "A" (65): with unsigned char,
233 - 65 > 0, so "\xE9" correctly sorts after "A". With signed char,
-23 - 65 < 0, so it would wrongly sort before "A". Using unsigned char gives
the same, correct order on every platform, like the standard strcmp.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
`sizeof buf` is 16 and `my_strlen(buf)` is 2.
`sizeof` gives the size of the whole array in memory. The compiler knows it at
compile time, and it does not care what is stored inside. `my_strlen` runs at
run time and counts the characters before the first '\0'. The array holds
'h', 'i', '\0', and the remaining 13 bytes are also 0, so it stops after 2.
In short, sizeof is the capacity of the buffer and strlen is how much text is
in it now. That is why for the size argument of my_strlcpy we pass
`sizeof buf`, not `my_strlen(buf)`.