# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
In C, arguments are passed by value: when the caller writes swap_i32(x, y), the values of x and y are copied into the parameters a and b, which are new, separate variables inside the function. The function swaps these copies, but the caller’s x and y are never touched, and when the function returns, the copies disappear. With pointers, the function receives copies of the addresses of x and y. The addresses are copies too, but they point to the caller’s original variables, so *a = *b writes directly into them.
## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
values is an input: the function only reads the array and must not change the caller’s data. const makes that a promise that the compiler enforces, and it tells the reader that the array is safe. It also allows passing a const array, for example a table stored in flash. min_out and max_out are outputs, so the function must write to them, and they cannot be const. If I wrote values[0] = 0;, the compiler would stop with an error (something like “assignment of read-only location”), not just a warning, so the program would not build.
## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
Imagine a temperature driver temp_read(&value). The main loop keeps the last good reading in value and calls temp_read every second; if the read fails, the plan is to keep using the old value. Now suppose the function sets *value = 0 at the start and only then discovers a sensor timeout and returns an error. The last good reading is destroyed, and the controller now thinks the room is at 0 °C, so it turns the heater on at full power. If the function had not touched the output on error, the old valid value would still be there and the device would keep working safely.
## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
A special error value only works if that value can never be a real result, and often every value is valid. A temperature of -1 is a real reading, and in min_max_i16 any int16_t value can be a real minimum, so there is no number left to mean “error”. Some functions also return more than one result, like quotient and remainder. With a separate status code, data and errors can never be confused, the function can report different kinds of errors (NULL pointer, timeout, bus error), and all functions in the API follow the same pattern, which makes the code easier to read and check.
