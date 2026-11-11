# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
The literal `1` is a signed `int` (32 bits). `1 << 31` would need the value 2^31, which does not fit in a signed int: the 1 is shifted into the sign bit, and in C that is undefined behaviour. UBSan reports something like "left shift of 1 by 31 places cannot be represented in type 'int'". With `(uint32_t)1u` the shift is done on an unsigned 32-bit type, where shifting by 0..31 is fully defined, so `BIT(31)` is exactly `0x80000000u`.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
A macro is pure text substitution. If a parameter appears more than once in the macro body (like `pos` in `FIELD_SET`), `i++` is pasted in several times, so `i` is incremented more than once, possibly in the same expression without a sequence point (undefined behaviour). Without parentheses the surrounding operators change the meaning: `~BIT(3)` becomes `~1u << 3` = `0xFFFFFFF8` instead of `~0x8`, and an argument like `a & b` becomes `(1u << a) & b`. A function evaluates each argument exactly once, before the call, converts it to the parameter type, and returns one value, so there is no textual pasting and no precedence surprise.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
1. Mask: `FIELD_MASK(16, 8)` = `0x00FF0000` (8 ones starting at bit 16).
2. Clear the field: `0x12345678 & ~0x00FF0000` = `0x12005678`.
3. Move the new value into place: `0xAB << 16` = `0x00AB0000`, and AND it with the mask so extra bits of `val` cannot leak outside the field.
4. Combine: `0x12005678 | 0x00AB0000` = `0x12AB5678`.

The field must be cleared first because OR can only turn bits on, never off. Without clearing, the old value `0x34` would mix with the new one: `0x34 | 0xAB` = `0xBF`, not `0xAB`.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
Before any arithmetic, `uint8_t` is promoted to `int`. So `x << 4` is an `int` that can be up to `0xFF0` (12 bits), and the whole expression is an `int`. Returning it as `uint8_t` is a narrowing from `int` to 8 bits. Actually the value does not even fit: for `0xA5` the result is `0xA5A`, and we rely on the conversion to drop the upper bits. The compiler cannot prove the value fits, so it warns. Writing an explicit `(uint8_t)` cast states that dropping the high bits is intended.