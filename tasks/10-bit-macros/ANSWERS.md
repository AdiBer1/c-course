# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
The literal `1` is a signed `int`. On a 32-bit `int`, bit 31 is the sign bit, and shifting a 1 into it gives a value (2^31) that a signed `int` cannot hold, which is undefined behaviour. UBSan reports it at runtime with a message like "left shift of 1 by 31 places cannot be represented in type 'int'". Even when it "works", the result is a negative `int` (`INT_MIN`), which then gets sign-extended if it is converted to a wider type, setting many more bits than intended. `(uint32_t)1u` is unsigned and exactly 32 bits, so shifting it by 0..31 is always defined, and `1u << 31` is simply 0x80000000u.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
A macro is plain text replacement: the preprocessor pastes the argument text into the body. If a parameter appears more than once in the body, `i++` is pasted (and executed) more than once, so `i` is incremented twice, or, without a sequence point in between, the behaviour is undefined. Without parentheses, operator precedence breaks the expression: `BIT(1 + 2)` becomes `1u << 1 + 2`, and since `+` binds tighter than `<<` it happens to be `1u << 3` here, but `~BIT(3)` becomes `~1u << 3`, which is `(~1u) << 3` = 0xFFFFFFF0 instead of ~0x8 = 0xFFFFFFF7. A function evaluates each argument exactly once before the call and receives its value, and its body is a separate expression, so neither problem can happen.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
The field is 8 bits starting at bit 16, so the mask is `FIELD_MASK(16, 8)` = 0x00FF0000.
1. Clear the old field: `0x12345678 & ~0x00FF0000` = `0x12345678 & 0xFF00FFFF` = 0x12005678.
2. Move the new value into place: `0xAB << 16` = 0x00AB0000, then `& 0x00FF0000` (removes any bits of val that do not fit in 8 bits) = 0x00AB0000.
3. Combine: `0x12005678 | 0x00AB0000` = 0x12AB5678.
The field must be cleared first because OR can only turn bits on, never off. Without clearing, the result would be `0x34 | 0xAB` = 0xBF in that field, a mix of the old and new value: 0x12BF5678 instead of 0x12AB5678.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
Because of integer promotion, `x` is converted to `int` before the shifts, so the expression `(x << 4) | (x >> 4)` has type `int`, and its value does NOT fit in 8 bits: for x = 0xA5 it is 0xA50 | 0x0A = 0xA5A. Returning that from a function declared to return `uint8_t` is an implicit narrowing conversion that throws away the upper bits, and that is exactly what `-Wconversion` warns about. Here dropping the upper bits is what we want (0xA5A becomes 0x5A), but the compiler cannot know that, so I wrote the cast `(uint8_t)(...)` explicitly to show the narrowing is intentional.