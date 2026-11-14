# Task 05 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why can't `void swap_i32(int32_t a, int32_t b)` (without pointers) swap the caller's variables? Explain what is copied where.

Answer:
Since there is no `*` before the variables in the function signature - meaning they are passed by value - a new variable is created that exists only for the duration of the function call, holding the same value as the original. The caller's variables `x` and `y` stay in the caller's stack frame, while their values are copied into `a` and `b`, which live in the stack frame of `swap_i32`. Consequently, modifying this variable does not affect the original variable: the function swaps only its own copies. When the function returns, its stack frame is released and the swapped copies are lost, so `x` and `y` keep their original values. With pointers, the address is copied instead of the value, and `*a = ...` writes directly into the caller's memory.

## Q2. Why does `min_max_i16` take `const int16_t *values` but `int16_t *min_out` without `const`? What would the compiler do if you tried to write `values[0] = 0;`?

Answer:
The `values` parameter represents an address of the input array. We do not want the data at this address to be modifiable - the function only needs to read the array. Note that `const int16_t *` protects the values the pointer points to, not the pointer itself: `values = values + 1;` compiles, but `values[0] = 0;` does not (a constant pointer would be written `int16_t * const p`). In contrast, `min_out` and `max_out` are intended to be provided by the caller to store results derived from the array values. They are output parameters, so the function must write to them, and therefore they cannot be `const`. If we attempt to modify the value at `values[0]`, the compiler will throw an error: `assignment of read-only location '*values'`, and the build fails.

## Q3. The rule "on error, do not write to the outputs" seems strict. Describe a situation in firmware where breaking it causes a real bug.

Answer:
Suppose there is a battery-powered device that calls the function `battery_read(&percent)` to read the charge level from a fuel-gauge chip over I2C. The I2C transfer fails because of electrical noise. However, the driver has already written the value `0` to `*percent` before reporting the error. If the calling code fails to check the status (or checks it too late), it will use `0` as the charge level, conclude that the battery is empty, and shut the device down immediately - possibly in the middle of saving data to flash, which corrupts it. Conversely, if the driver leaves the output value unchanged in the event of an error, the calling code can continue using the last valid reading - a much safer approach.

## Q4. Why do embedded APIs usually return a status code and put the result in an output parameter, instead of returning the result and using a special value like -1 for errors?

Answer:
Because it is difficult to find a value that always signifies an error. Suppose we decide that -1 indicates an error. However, in some cases - such as reading a temperature - a value of -1 might actually be a valid result. In such a scenario, there would be no way to signal an error, or a valid result might be incorrectly interpreted as an error. Furthermore, when using unsigned types (such as `uint16_t`), there is no "spare" value available to indicate an error, which could lead to a conflict between error codes and the data itself. For example, a 12-bit ADC stored in a `uint16_t` returns only 0..4095, so a value like 0xFFFF happens to be free, but a 16-bit ADC uses the whole range 0..65535 and leaves nothing free; a status code works the same way for both. Separating the data from the error codes ensures the integrity of the data and also makes it possible to identify the specific error that occurred, rather than merely indicating that an error took place.
