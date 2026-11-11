# Task 05 — Answers

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
In C, arguments are passed by value, meaning the function receives temporary copies of the caller's variables rather than the actual variables themselves. Swapping those copies inside the function only modifies the local copies and has no effect on the original variables in the caller's stack frame. Pointers are required so the function can access and modify the original memory addresses.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
The `const` qualifier on `values` tells the compiler that the function will only read from the input array and never modify its contents, ensuring safety and allowing the caller to pass read-only data. `min_out` lacks `const` because it is an output parameter meant to be written to. If you tried to write `values[0] = 0;`, the compiler would issue a compilation error because modifying a `const`-qualified pointer target is illegal.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
If a firmware function fails halfway through but has already partially overwritten an output variable with garbage or stale data, and the caller fails to check the return status code and uses that output anyway, the system will operate on invalid data. In embedded systems, using corrupted sensor or configuration data can lead to dangerous hardware behavior, actuator failures, or system crashes.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
Returning a special error value (like -1) limits the valid data range of the function, as that value might be a legitimate result. Returning a status code via the function's return value separates success/failure diagnostics from the actual data payload, allowing functions to return multiple types of specific error codes (like null pointers, division by zero, or empty inputs) while returning rich results through pointers.