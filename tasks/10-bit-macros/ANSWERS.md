# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
1 is a signed int, and bit 31 is its sign bit. So 1 << 31 puts a 1 into the sign bit, and that is undefined behaviour. UBSan would say something like “left shift of 1 by 31 places cannot be represented in type ‘int’”. (uint32_t)1u has no sign bit, so shifting it by any number from 0 to 31 is always fine, on every chip.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
A macro just copies text. If the parameter is used more than once inside the macro, i++ is copied more than once and i goes up several times. Without parentheses, the order of operations gets mixed up: BIT(1 + 2) happens to work, but BIT(3) + 1 becomes 1u << 3 + 1, which is 16 instead of 9. A function doesn’t have these problems, because it calculates each argument once and only gets its value.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
The mask is 0x00FF0000 (bits 16–23). First I clear the field: 0x12345678 becomes 0x12005678. Then I move 0xAB into place: 0x00AB0000. Then I combine them with OR: 0x12AB5678. I must clear first because OR can only turn bits on, not off. Without clearing, the old 0x34 and the new 0xAB mix into 0xBF, and the result is wrong.

## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
Before the shift, x becomes an int, so the result is an int, and turning it back into uint8_t can lose data. And it really does: for 0xA5 the result is 0xA5A, which doesn’t fit in 8 bits. The function works only because the extra bits are cut off. So the warning is correct, and the cast (uint8_t) shows that cutting them is on purpose.

