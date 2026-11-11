# Task 10 — Answers

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?
Answer: `#define BIT(n) (1 << (n))` uses a signed 32-bit integer, and shifting into or past the sign bit (such as for $n = 31$) results in undefined behavior. Using `(uint32_t)1u` ensures unsigned arithmetic, allowing bit 31 to be set safely without triggering sanitizer or compiler overflow warnings.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?
Answer: Unparenthesised macros suffer from operator precedence issues (e.g., `1u << 1 + 2` evaluates incorrectly), and macros that evaluate parameters multiple times cause side effects to execute repeatedly when passing expressions like `i++`. Functions do not have these problems because their arguments are evaluated exactly once prior to invocation.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?
Answer: First, `FIELD_SET` generates a mask for the specified position and width and clears those bits in the register using a bitwise AND with the inverted mask. Then, it shifts the new value into place and merges it using a bitwise OR. It must clear the field first to prevent leftover bits from the old value from corrupting the new field value.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?
Answer: In C, integer promotions cause `uint8_t` operands to be promoted to `int` during bitwise operations, making the resulting expression type an `int`. Returning an `int` from a function returning `uint8_t` triggers a potential narrowing conversion, which `-Wconversion` flags unless explicitly casted.