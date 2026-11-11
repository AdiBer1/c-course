# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
When integer promotion applies to types smaller than `int` (such as `uint8_t`), operands `a` and `b` are automatically promoted to `int` before the addition is performed.

* **Type of the expression `a + b`:** **`int`**
* **Value of the expression `a + b`:** **`300`**
* **What happens when you store it back into a `uint8_t`:** Because `uint8_t` can only hold values from 0 to 255, storing 300 back into it causes wrapping (truncation via modulo 256), resulting in **`44`**.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
Standard types (`int`, `short`) have sizes that vary across architectures and compilers. `stdint.h` types guarantee exact bit-widths, which is critical for hardware interaction and portability.

**Concrete case:** Assuming an `int` is 32 bits to parse a fixed-size network packet, but compiling on a 16-bit microcontroller where `int` is 16 bits, causes severe data misalignment and incorrect buffer parsing.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
Unsigned overflow has a well-defined wrap-around behavior, whereas signed overflow is Undefined Behavior (UB), allowing compilers to aggressively optimize away safety checks. `sat_add_i16` avoids ever overflowing by checking the operands for potential bounds violations *before* performing the addition, clamping the result to `INT16_MAX` or `INT16_MIN` safely.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
When `level` is 5, subtracting 10 results in $-5$. Because `uint8_t` cannot hold negative numbers, an unsigned underflow occurs, wrapping the value around to 251 (nearly full battery!). Saturation fixes this by clamping the result to the minimum allowed value (0) instead of wrapping.
