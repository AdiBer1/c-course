# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
Without pointers, the function receives copies of the original values. It changes only these copies and not the original variables. With pointers, the function can access and change the original variables.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
const means that the function can read the input array but cannot change it. The output pointers do not have const because the function needs to write the results into them. If we try to write 0 in values indexed 0, the compiler will report an error.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
If a sensor reading fails, the function might write zero to the output. The program may think that zero is a real sensor reading and make a wrong decision. 

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
A status code tells us if the operation was successful or which error happened. The output parameter can store the actual result. This is useful because sometimes all values, including -1, can be valid results, so we cannot always use a special value to indicate an error.
