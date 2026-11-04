# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
The type of the expression a + b is int due to integer promotion, and its value is 300.
When storing the result back into a uint8_t, a wrap-around occurs and the actual stored value will be 44 (since 300 minus 256).

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
Embedded systems require absolute and reliable control over memory size and the population of bits in every variable, without depending on the processor's architecture.
Using a plain int type might surprise  when porting the code to a different processor (example- from 32-bit to 16-bit), as the variable's size changes and can cause overflow or mismatch issues when interacting with external hardware.


## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
Signed overflow is undefined behavior according to the C standard, which allows the compiler to make incorrect optimizations that lead to crashes or severe bugs in real code.
In order to solve the problom the sat_add_i16 function casts the variables to a wider type (int32_t) before adding, checks in advance if the result exceeds the bounds of int16_t, and in case of overflow, clamps the value precisely to the maximum or minimum limit without ever wrapping around.


## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
 When the battery level is 5 and the code subtracts 10 from it, the true result is -5. Because it is an unsigned type, the value sharply wraps around to a massive number (251).
 Saturation detects that the result dropped below zero and immediately clamps it at the minimum legal value of 0, thereby preventing the erroneous value jump.

