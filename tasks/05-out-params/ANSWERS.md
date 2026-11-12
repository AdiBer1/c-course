# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
Without pointers, C only copies the values into local variables inside the function. The swap happens only inside the function, so the original variables in the main code don't change at all.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
values has const because we only read from it and shouldn't change the array. min_out has no const because we need to write the result into it. If we try to change values[0], the compiler will give an error.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
If a sensor fails and writes bad or old data to the output before returning an error, the program might use that bad data. This can cause a real bug or make the hardware do something dangerous.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
Because any number (like -1 or 0) can sometimes be a real, valid reading from a hardware sensor. Using a status code separates the success/failure check from the actual data.
