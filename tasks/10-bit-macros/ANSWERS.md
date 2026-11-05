# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
`1` is a signed 32-bit `int`, so shifting it left by 31 bits shifts a bit into the sign bit, which causes signed integer overflow (Undefined Behavior in C). UBSan flags this as an illegal signed shift overflow. Using `1u` makes the constant `unsigned int`, where bitwise left shifts are fully defined, and casting to `(uint32_t)` ensures a guaranteed 32-bit unsigned type.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
Macros use raw text substitution. If an argument with side effects like `i++` is passed to a macro that evaluates its parameter multiple times, `i` will be incremented multiple times. If parameters or expressions lack parentheses, operator precedence can alter the evaluation order (e.g., `~1u << 3`). Functions avoid this because arguments are evaluated exactly once before invocation (pass-by-value) with strong type safety and explicit scope.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
It must clear the field first because bitwise OR (`|`) can only set bits to 1; it cannot turn an existing 1 bit into a 0 bit. Without clearing `0x34` at bits 16..23, performing `OR` with `0xAB` would corrupt the output (`0x34 | 0xAB = 0xBF`). Step 1: invert the field mask (`0xFF00FFFF`) and `AND` with reg to get `0x12005678`. Step 2: mask `val` to 8 bits and shift left by 16 to get `0x00AB0000`. Step 3: `OR` them together to get `0x12AB5678`.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
In C, integer promotion automatically promotes types smaller than `int` (like `uint8_t`) to a signed 32-bit `int` before performing bitwise operations. Thus, `(x << 4) | (x >> 4)` evaluates to a 32-bit `int`. Returning this value from a function returning `uint8_t` is an implicit narrowing conversion. Under `-Wconversion`, the compiler flags any implicit narrowing, requiring an explicit `(uint8_t)` cast to acknowledge the operation.

