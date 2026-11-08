# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
Because in C the arguments moved to the fanctions by value and each change is on the copy, when we transfer pointers the changes will realy stey also out of the function.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
values is just input and const promis that the func will not change him, min_out is output and the func have to change him.
The attempt `values[0] = 0;` will cause a compilation error.
## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
Suppose the caller initializes `uint16_t temp = last_good_value;` and calls `adc_read(ch, &temp)`. If the function fails but writes a garbage value (e.g., 0) to the output, the caller might ignore the error code—or check it too late—and end up using 0 as the actual temperature. Consequently, a heating controller might incorrectly perceive the temperature as too low and activate full heating. If, however, the output remains unchanged in the event of an error, the caller continues using the last valid value.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
A special value like -1 might also be a valid result, making it impossible to distinguish between a result and an error. Furthermore, a separate status code always clarifies whether the call succeeded, allows for the return of different types of errors, and enables returning multiple results simultaneously.

