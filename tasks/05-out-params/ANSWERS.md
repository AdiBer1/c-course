# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
In C every argument is passed by value. When the caller writes `swap_i32(x, y)`, the values of `x` and `y` are copied into new local variables `a` and `b` in the function's own stack frame. The function swaps those two copies, and when it returns the copies disappear, while `x` and `y` in the caller's frame were never touched. With pointers, the caller passes `&x` and `&y`. What gets copied now is the addresses, and through `*a` and `*b` the function reads and writes the caller's real variables.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
`values` is an input: the function only reads the array, and `const` is a promise to the caller that their data will not be changed. It also lets the caller pass a constant array, for example one stored in flash. `min_out` and `max_out` are outputs: the whole point is that the function writes the results into them, so they cannot be `const`. If I wrote `values[0] = 0;`, the compiler would refuse to compile it with an error like "assignment of read-only location", so the mistake is caught at compile time instead of on the device.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
Imagine a temperature controller that keeps the last good reading in `temp` and calls `sensor_read(&temp)` every second. If the sensor times out, the code keeps using the old value, which is still close to correct. If `sensor_read` wrote a half-finished value or 0 into `temp` before it discovered the timeout, the controller would suddenly "see" 0 degrees and turn the heater on at full power, even though the real temperature never changed. Leaving the outputs untouched on error means the caller can always trust that its variable holds either a new valid value or the old valid one.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
A special value like -1 only works if that value can never be a real result, and often every value is valid: an `int16_t` temperature can really be -1, and a `uint16_t` ADC reading has no negative numbers at all. A separate status code keeps "did it work" apart from "what is the value", so any value can be a valid result. It also allows several different errors (NULL pointer, empty array, timeout, division by zero) to be reported clearly, and the same status type can be used consistently across all the drivers in a project.