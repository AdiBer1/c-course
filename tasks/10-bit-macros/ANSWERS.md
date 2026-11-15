# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
Because 1 is a signed int, shifting it to bit 31 can cause undefined behavior. Using uint32_t makes the value unsigned and avoids this problem.
## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
A macro can evaluate an argument more than once, so i++ may be executed multiple times. Missing parentheses can also cause unexpected results because of operator precedence. Functions evaluate their arguments only once.
## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
First, it creates a mask for the 8 bits at position 16. Then it clears the old field, shifts 0xAB to the correct position, and combines the values using OR. It must clear the old field first so the previous bits do not affect the new value.
## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
Because uint8_t is promoted to int when used in these operations. The result is an int, and converting it back to uint8_t may lose bits. An explicit cast tells the compiler that this conversion is intentional.
