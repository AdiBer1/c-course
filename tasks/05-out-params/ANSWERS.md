# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
C passes arguments by value. When calling `swap_i32(a, b)`, local copies of `a` and `b` are created on the stack for the function scope. Modifying these local parameters inside the function swaps only the copies, leaving the caller's original variables untouched.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
`values` is marked `const` to guarantee that the function only reads from the input array without modifying its contents, while `min_out` lacks `const` so the function can write the computed result back. If you try to execute `values[0] = 0;`, the compiler will raise a compilation error for attempting to modify read-only memory.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
If a sensor driver function writes partial garbage data to an output pointer before encountering a bus timeout or checksum error, the calling code might retain or use that corrupted value, potentially leading to incorrect hardware decisions or overwriting a previously valid measurement.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
Returning a status code cleanly separates error handling from valid data values. Using magic return values like `-1` fails when the entire integer range (e.g., negative temperatures or unsigned 16-bit ADC raw values) represents valid measurement output.
