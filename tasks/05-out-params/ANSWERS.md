# Task 05 — Understanding questions

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:

In C, function arguments are passed by value. This means the function receives copies of the caller's variables. If we swap `a` and `b` inside the function, we only change the copies, not the original variables. Pointers allow the function to access and change the original values.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:

The `values` pointer is const because the function only needs to read the array and should not modify it. The output pointers are not const because the function needs to write the minimum and maximum values into them. If I try to write `values[0] = 0`, the compiler will report an error because the data is read-only through a const pointer.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:

Imagine a sensor-reading function that fails because of a communication error. If it changes the output value before returning an error, the program might use an invalid sensor reading as if it were correct. This could cause another part of the system to make a wrong decision. Keeping the output unchanged helps prevent this problem.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:

A status code clearly tells the caller whether the operation succeeded or failed. The output parameter can then hold the actual result. This is useful because every possible value of the result can still be valid, so a special value like -1 might be confused with a real result. It also allows a function to return several results.
