# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
The expression a + b promotes to a standard int with a value of 300. When assigning 300 back to a uint8_t, it wraps around modulo 256 (300 (mod256)), resulting in 44.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
Standard C types like int change their bit-width depending on the hardware architecture. Explicit types like uint8_t or uint32_t guarantee fixed sizes across all compilers, preventing unexpected bitwise truncation or overflows.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
Signed overflow is Undefined Behaviour, allowing the compiler to break or optimize away safety checks. sat_add_i16 avoids overflow by casting operands to int32_t before arithmetic, performing the addition safely in 32 bits, and clamping the result to INT16_MIN / INT16_MAX before casting back.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
Evaluating 5 - 10 on an unsigned 8-bit integer causes an underflow, wrapping around to 251 (reporting a 98% battery level instead of empty). Saturation prevents this by clamping any negative result directly to 0

