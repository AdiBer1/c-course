# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
In C, `1` is a signed 32-bit integer (`int`), so `1 << 31` shifts a bit into the sign bit, which causes undefined behavior (overflow of a signed integer). UBSan and the compiler warn about shifting into the sign bit or producing a negative result. Using `1u` explicitly operates on `uint32_t`, making bit 31 a valid unsigned shift without undefined behavior.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
Macros perform raw text substitution. Passing `i++` to a macro that uses its parameter multiple times causes `i` to increment multiple times (side effects). Without proper parentheses, precedence issues occur: `BIT(1 + 2)` expands to `1u << 1 + 2`, which evaluates as `(1u << 1) + 2 = 4` instead of `1u << (1 + 2) = 8`. Functions do not have these problems because arguments are evaluated exactly once before execution and parameters have strict type and precedence rules.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
First, a mask for the field is generated (8 bits starting at position 16, giving `0x00FF0000`). Next, `reg` is bitwise ANDed with `~mask` to clear (zero out) the target field bits, resulting in `0x12005678`. Then, `val` (`0xAB`) is masked to fit 8 bits and shifted left by 16 bits to yield `0x00AB0000`. Finally, the cleared register and the shifted value are combined with bitwise OR to produce `0x12AB5678`. It must clear the field first because `OR`ing new bits into non-zero existing bits would merge old and new values instead of replacing them.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
In C, integer promotions automatically promote operands smaller than `int` (like `uint8_t`) to a signed `int` before performing bitwise shift or OR operations. Therefore, the expression `(x << 4) | (x >> 4)` evaluates to a 32-bit `int`. Returning this expression to a function returning `uint8_t` triggers `-Wconversion` because it attempts an implicit narrowing conversion from `int` back to `uint8_t`, requiring an explicit `(uint8_t)` cast.


