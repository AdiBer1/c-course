# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
1 is a signed integer, so shifting it into bit 31 causes undefined behavior because the result cannot be represented as a signed int. Using uint32_t makes the value unsigned, so setting bit 31 is valid. UBSan can report that the left shift cannot be represented in type int.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
A macro can evaluate an argument more than once, which may cause i++ to run multiple times. In our BIT_SET definition, n is used only once, so i++ is evaluated once. Also, missing parentheses can change how an expression is evaluated when combined with other operators. Functions avoid repeated evaluation of arguments because they are evaluated once when passed to the function.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
First, it creates a mask for bits 16–23: 0x00FF0000. Then it clears those bits in the original value, giving 0x12005678. Next, it shifts 0xAB into the correct position and combines it with the result. The final value is 0x12AB5678. Clearing the field first prevents the old bits from interfering with the new value.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
In C, uint8_t is promoted to int before the shift operations. The final expression is therefore an integer, and converting it back to uint8_t may lose bits. The compiler cannot always prove that the value fits, so an explicit cast is needed.
