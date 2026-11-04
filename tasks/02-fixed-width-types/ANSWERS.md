# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
`a` and `b` are promoted to `int` during evaluation, making the type of `a + b` an `int` with a value of `300`. When stored back into a `uint8_t`, it truncates to 8 bits, wrapping around to `44` (`300 % 256`).

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
Standard C types like `int` do not guarantee a fixed bit-width across hardware architectures. For example, on a 16-bit microcontroller (like AVR), `int` is 16 bits, whereas on a 32-bit processor or PC, `int` is 32 bits. Using `stdint.h` types guarantees identical size, memory layout, and behavior across all compilers and target devices.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
Signed overflow is Undefined Behavior, allowing the compiler to optimize away safety checks or produce subtle runtime bugs. Our `sat_add_i16` avoids overflowing by casting the operands to `int32_t` before addition, ensuring the arithmetic evaluated in 32-bit space never overflows 16 bits before clamping.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
Without saturation, subtracting 10 from 5 causes an unsigned underflow, wrapping around to `251` (indicating a nearly full battery instead of empty). Saturation detects underflow and clamps the value to `0`, correctly representing a fully depleted battery level.

