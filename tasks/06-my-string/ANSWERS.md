# Task 06 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. `char buf[4]; strcpy(buf, "LED=1");` compiles without error. What exactly happens in memory, and why can it crash much later in a completely different function?

Answer:
buf holds 4 bytes, but "LED=1" needs 6 (with \0), so strcpy writes 2 bytes out of bounds. This overwrites adjacent stack memory like nearby variables or return addresses without failing immediately. The program crashes later when using the corrupted memory, hiding the true cause.

## Q2. Why does `my_strlcpy` return the length of `src` rather than the number of bytes copied? Show how a caller uses that value.

Answer:
It returns src's full length so the caller can check if (ret >= dst_size) to detect truncation. It also tells the caller the buffer size needed for the full string. Returning only copied bytes would make it impossible to tell if the string was cut off.

## Q3. Why must `my_strcmp` compare characters as `unsigned char`? Give an example where using plain `char` gives the wrong order.

Answer:
Plain char can be signed, making values above 127 negative and breaking order (e.g., "\xC8" becomes -56, making it smaller than "A" at 65). Casting to unsigned char ensures correct comparison and consistent behavior on all systems.

## Q4. `sizeof buf` and `my_strlen(buf)` for `char buf[16] = "hi";` — what are the two values and why are they different?

Answer:
sizeof buf is 16 because it returns the total allocated array size set at compile time. my_strlen(buf) is 2 because it counts characters up to \0 at runtime. sizeof measures the container; strlen measures the content.
