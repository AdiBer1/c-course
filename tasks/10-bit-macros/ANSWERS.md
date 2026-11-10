# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
Literal 1 is a signed int (32-bit signed). Shifting 1 << 31 shifts a 1 into the sign bit, which is Undefined Behaviour in C. Using (uint32_t)1u << n ensures unsigned arithmetic, making bit shifts up to bit 31 fully defined and safe.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
Macro parameters are evaluated using direct text replacement. In BIT_SET(reg, i++), the argument i++ gets evaluated multiple times, causing side effects to execute more than once. Without parentheses, BIT(1 + 2) expands to 1u << 1 + 2, which evaluates as (1u << 1) + 2 due to operator precedence instead of 1u << 3. Functions evaluate arguments exactly once before execution and strictly respect scope boundaries.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
Answer:FIELD_SET first clears the target bitfield using reg & ~FIELD_MASK to zero out the destination bits. It then masks val to the allowed width, shifts it to pos, and combines it using bitwise OR (|). Clearing first is required because bitwise OR can only turn 0s into 1s; if existing 1s are not cleared, they will corrupt the new value.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
In C, integer promotion automatically converts uint8_t operands to int before bitwise shifts. The expression (x << 4) | (x >> 4) produces a 32-bit int result. Assigning or returning it as uint8_t triggers a -Wconversion compiler warning due to implicit narrowing, requiring an explicit (uint8_t) cast.
