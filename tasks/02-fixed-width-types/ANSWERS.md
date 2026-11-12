# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer: 
Both operands are promoted to `int` (integer promotion), so `a + b` has type `int` and the value 300. When 300 is stored back into a `uint8_t`, the value is reduced modulo 256 and becomes 44 (300 - 256), because 300 does not fit in 8 bits.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer: 
The sizes of `char`, `short` and `int` are implementation-defined and differ between platforms (`int` can be 16 bits on an 8/16-bit MCU and 32 bits on an ARM core), and plain `char` may be signed or unsigned. The fixed-width types guarantee exact size and signedness, which matters for registers, protocols and memory layouts. For example, code that assumes `int` is 32 bits, such as `int x = 40000;` or `x * x`, breaks or overflows on a platform with a 16-bit `int`, and `char c = 200;` may be negative on one compiler and 200 on another.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
Unsigned overflow is defined to wrap modulo 2^N, so the result is predictable. Signed overflow is undefined behaviour: the compiler may assume it never happens, so it can optimize away overflow checks, produce unexpected results, or behave differently between compilers and optimization levels. The bug may appear only in release builds and is very hard to debug. My `sat_add_i16` avoids overflow entirely by widening both operands to `int32_t` before adding. `int32_t` has a fixed width of 32 bits, so this is safe even on platforms where `int` is 16 bits, and the sum of two 16-bit values always fits in 32 bits. Only then do I compare the result against `INT16_MAX` and `INT16_MIN` and clamp it, and the cast back to `int16_t` happens only when the value is already known to be in range.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer: 
`level` is promoted to `int`, so `level - 10` is computed as 5 - 10 = -5. Storing -5 in a `uint8_t` wraps modulo 256 and gives 251, so a nearly empty battery looks almost full, which is a dangerous logic error. Saturation fixes it by clamping the result at the minimum: if `level < 10` the result is 0 instead of wrapping around. In my code, `sat_sub_u8(level, 10)` compares before subtracting and returns 0 when `level < 10`, so the value always stays meaningful.git --no-pager diff -- tasks/02-fixed-width-types/ANSWERS.md