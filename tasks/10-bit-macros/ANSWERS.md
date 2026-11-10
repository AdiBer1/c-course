# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
`1` is a signed int, so shifting it into bit 31 causes undefined behavior. Using `uint32_t` makes the shift unsigned. UBSan can report that the signed left shift cannot be represented.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
A macro may evaluate an argument more than once, so `i++` can change the value unexpectedly. Missing parentheses can also change operator precedence. Functions evaluate each argument once and follow normal expression rules.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
The mask is `0x00FF0000`. First, the old field is cleared, then `0xAB` is shifted left by 16 and inserted. The result is `0x12AB5678`. Clearing first prevents old bits from remaining set.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
`uint8_t` is promoted to `int` during the operations. The result is an `int`, which is then narrowed to `uint8_t`. An explicit cast tells the compiler that the conversion is intentional.
