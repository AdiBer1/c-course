# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
C passes arguments by value. When the function is called, the values of the caller's variables are copied into two new local variables, `a` and `b`, which live on the function's stack frame. The swap exchanges only those copies, and they disappear when the function returns, so the caller's variables are never touched. With pointers, the function receives the addresses of the caller's variables and can write directly to the caller's memory.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
`values` is input only: the function just reads it, and `const` is a promise not to modify the caller's array. `min_out` is an output parameter, so the function must write through it, which means it cannot be `const`. Writing `values[0] = 0;` would be a compile error (assignment of a read-only location). `const` also documents the intent and allows passing read-only arrays, for example data stored in flash.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
Suppose `adc_read(ch, &value)` fails with a timeout but has already written 0 or garbage into `value`. A caller that ignores or checks the status too late uses that value as if it were a real reading. A temperature controller could then see 0 degrees, assume it is freezing, and switch on a heater. If the output is left untouched on error, it still holds the last valid value, which is much safer.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
Often the whole range of return values is valid: -1 can be a real temperature or a legitimate ADC reading, so a result cannot be told apart from an error. A separate status code also lets the function report different kinds of errors (timeout, bus error, NULL pointer) and makes it easy to enforce one consistent check on every call. Separating "did it work" from "what is the result" avoids this ambiguity.