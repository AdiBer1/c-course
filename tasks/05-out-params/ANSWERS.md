# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
C passes values by copy, so the function only changes its local copies. Pointers let the function access and change the original variables.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
The values array is only for reading, so it uses const. The output pointers need to be changed to store the results. The compiler would report an error if I tried to modify the array.


## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
For example, if a sensor read fails but the function still changes the output, the program might use an incorrect value. Keeping the output unchanged prevents this kind of bug.


## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
A status code clearly tells us if the operation succeeded. An output parameter can hold the full result range, without reserving a value like -1 for errors.
