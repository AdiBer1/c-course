# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
C passes parameters by value, meaning the function receives local copies of a and b. Modifying these copies inside swap_i32 only changes the stack variables inside the function and does not affect the caller's variables. Pointers are required to directly access and modify the caller's memory locations.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
const int16_t *values indicates that the function only reads from the array and promises not to modify it, whereas int16_t *min_out is a write target for the result. If you attempt values[0] = 0;, the compiler will throw a compilation error for attempting to modify read-only memory.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
If a function updates an output buffer before encountering an error (such as a timeout), the caller might inspect or keep stale/partially overwritten garbage data, leading to incorrect calculations or safety hazards (like driving an actuator with corrupt values).

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
Using magic return values (like -1) restricts the valid numeric range of data (e.g., if -1 is a valid sensor value). Returning a status enum decouples error handling from data, allowing the full type range for valid data while forcing explicit handling of status codes.
