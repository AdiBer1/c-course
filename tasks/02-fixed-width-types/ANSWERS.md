# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
- **Type and Value of `a + b`:** `int` with value `300` (due to C's integer promotion rules).
- **Storing back to `uint8_t`:** Truncates modulo 256 ($300 \pmod{256}$), resulting in `44`.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
- **Why fixed-width types:** Standard C types like `int` vary in width depending on architecture. `<stdint.h>` guarantees exact bit widths across all compilers and platforms.
- **Concrete surprise:** `30000 + 10000` stored in `int` produces `40000` on a 32-bit MCU (or PC), but overflows to `-25536` on an 8-bit MCU where `int` is 16 bits.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
- **Why it is dangerous:** Because signed overflow is Undefined Behavior (UB), compilers assume it never happens. Optimizers may silently remove post-addition overflow checks or alter program logic unexpectedly.
- **How `sat_add_i16` avoids overflow:** It checks bounds using algebra before executing the addition (e.g., checking `a > INT16_MAX - b` when `b > 0`). The expression `a + b` is only evaluated when it is guaranteed not to overflow.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
- **What goes wrong:** `5 - 10` evaluates to `-5`, which wraps around modulo 256 when assigned back to `uint8_t`, producing `251`. The system incorrectly reads a depleted battery as nearly full.
- **Why saturation fixes it:** `sat_sub_u8` detects that `5 < 10` and clamps the output to `0` instead of wrapping, correctly indicating a fully discharged battery.