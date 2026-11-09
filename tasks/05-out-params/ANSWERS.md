# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
In C, arguments are passed by value. This means the function receives copies of the caller's variables, not the original variables themselves. Modifying `a` and `b` inside the function only changes those local copies, while the original variables in the caller remain unchanged.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
The `values` parameter has `const` because it is an input buffer that the function should only read from without modifying. In contrast, `min_out` and `max_out` are output parameters that the function must write the results into, so they cannot be `const`. If we tried to write `values[0] = 0;`, the compiler would generate a compilation error for attempting to modify read-only memory.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
Suppose a firmware task reads a sensor and passes an output variable holding the previous valid reading (e.g., motor speed or temperature). If the driver function encounters a failure (like a timeout) but partially writes dummy or corrupted data into the output variable before returning an error, a caller thread that forgets to check the return code might act on that invalid data and trigger dangerous hardware actions.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
Using a status code allows returning rich diagnostic errors while keeping the data path clear. If a function returns the calculated value directly, every possible return value (including negative numbers like -1 or zero) might be a valid result, making it impossible to distinguish between a real answer and an error. Passing outputs via pointers cleanly separates execution status from data processing.
