# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
buf has 4 bytes, but "LED=1" needs 6 bytes (5 chars + the '\0'). strcpy
doesn't know the size of buf, so it keeps copying and writes past the end
of the buffer, overwriting whatever is next to it in memory (other
variables, the return address). Nothing breaks at that moment, so the
program crashes later, in a different function that uses the corrupted
memory.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
Returning the length of src lets the caller know if the string was cut.
If the returned value is >= dst_size, the copy was truncated. If it
returned only the number of bytes copied, the caller couldn't tell.

if (my_strlcpy(buf, s, sizeof buf) >= sizeof buf) { /* truncated */ }

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
Plain char can be signed, so a byte like 0xE9 (233) becomes negative (-23).
The correct order is by byte value 0-255. For example, comparing"\xE9" and "a": as unsigned, 233 > 97, so the first sorts after "a" (correct).
As signed char, -23 < 97, so it sorts before "a" (wrong).

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
sizeof buf is 16 and my_strlen(buf) is 2. sizeof is the size of the whole array in bytes.
my_strlen counts the chars until the first '\0', and here that is only 'h' and 'i'.
