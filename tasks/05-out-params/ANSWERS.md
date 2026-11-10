# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
The function gets copies of the values, not the original variables. If it changes them, only the copies change. With pointers, the function can access and change the original variables.


## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
The function only needs to read the values in the array, so it uses const. The output pointers are not const because the function needs to write the results into them. If we try to change values[0], the compiler will give an error.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
For example, a function tries to read the temperature from a sensor, but the reading fails. If it changes the output value anyway, the program might use a wrong temperature. This could make the system do something it should not do.


## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
Because -1 can also be a real result. The status code tells us if the function worked, and the output parameter gives us the result. This way, we can tell the difference between a result and an error.

