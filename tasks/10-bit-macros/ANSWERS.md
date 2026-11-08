# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
In this operation the 1 is shifted all the way to bit 31. In a signed type this bit is reserved for the sign, so the value that reaches it is too big and cannot be represented in an `int`, which is undefined behavior, and the value is wrong. If we use an `unsigned` value instead, bit 31 is free and can hold values. If we did the operation on a signed `int`, the error we would get is something like:
`runtime error: left shift of 1 by 31 places cannot be represented in type 'int'`

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
1. Every time the variable appears, the macro replaces the text with what we wrote for it, so every time the variable appears the `++` operation is performed again. This gives a value we didn't want.
2. Without parentheses, the macro doesn't know in which order to perform the operations: the text gets mixed with everything around it.

Functions don't have this problem, because they receive a value and not text. The argument that is passed is computed only once before each call, and a function returns a final value, so there is no mixing with what is around it.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
First, we prepare a mask: 1 on all the bits where we want to put the new value. Second, we invert the mask. Third, we clear those bits: we do AND between the original and the inverted mask, so that all the bits where we want to put the new value become 0. After that we create a value with the wanted number `0xAB`, shift `0xAB` to bit 16, and do OR with the original, and then from bit 16 onward the original holds the wanted value.
Why must we clear first? OR only turns bits on, it doesn't turn them off. So if we don't clear, and there is a 1 in a place where we now don't want a 1, that is a problem.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
Because of the shift operators `<<` and `>>`, the value of `x` is promoted to `int` (integer promotion), so the calculation is done in `int`. After the calculation, the function converts it back to `uint8_t`. It is possible that the value really fits in this type, but the compiler warns that the conversion is to a smaller type, and information might be lost.
