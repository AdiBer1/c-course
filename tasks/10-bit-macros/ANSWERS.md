# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
The literal 1 is a signed int (typically 32-bit), and shifting a 1 into or past the sign bit (bit 31) causes signed integer overflow, which is Undefined Behavior (UB) in C. Using 1u casts the operand to an unsigned type (uint32_t), where left-shifting into the most significant bit is well-defined. UBSan reports a runtime error similar to: left shift of 1 by 31 places cannot be represented in type 'int'.
## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
Macros perform blind text substitution before compilation: if a macro argument has side effects (like i++) and appears multiple times in the macro body, it gets evaluated multiple times, causing unintended increments. Similarly, without parentheses, BIT(1 + 2) expands to 1u << 1 + 2, which evaluates as (1u << 1) + 2 due to operator precedence (+ binds tighter than <<). Functions avoid both issues because their arguments are fully evaluated exactly once into typed values before entering the function body.
## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
Create mask: Build an 8-bit mask (0xFF) and shift it left by 16 bits to get 0x00FF0000.

Clear field: Bitwise-AND the original value with the inverted mask (0x12345678 & ~0x00FF0000), zeroing out bits 16–23 to yield 0x12005678.

Position new value: Mask the new value to 8 bits and shift it left by 16 ((0xAB & 0xFF) << 16), giving 0x00AB0000.

Insert value: Bitwise-OR the cleared register with the shifted value (0x12005678 | 0x00AB0000) to get the final result 0x12AB5678.

It must clear the field first because bitwise OR (|) can only set bits to 1, never reset existing 1s to 0—without clearing first, old bits in that field would remain set and corrupt the new value.
## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
Due to C's integer promotion rules, operands narrower than int (like uint8_t) are automatically promoted to int before arithmetic or bitwise operations are performed. Consequently, the expression (x << 4) | (x >> 4) has the type int (and (x << 4) actually shifts bits into bits 8–11 of that int, making the intermediate value larger than 255). -Wconversion warns whenever a wider type (int) is implicitly converted back to a narrower type (uint8_t) without an explicit cast, because the compiler's type rules cannot guarantee at the type level that upper bits aren't lost.
