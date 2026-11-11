# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
Because just 1 is a signed number int, so the MSB is the sign bit - plus or minus, so assigning into the 31 bit is wrong - this is the sign bit, not part of the number, the result might be the opposite signed instead of +2^31. this is an undefined behavior.

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
The pre-compiler just takes every time of the i++ and copies the code, it uses i++ just as a parameter like n, so it can get to x1=(x++)+(x++), this is an undefined behavior, like without paranthesised - it is un-clear what happens first, for example ~BIT(3) becoming (~1u) << 3, A function evaluates its arguments once, passes them as values, and returns a single value, so none of this happens.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
accurding to the example
1 mask= FIELD_MASK(16, 8) = 0x00FF0000 - this makes the part of the field 1's
2.~mask= 0xFF00FFFF
3.reg & ~mask  = 0x12005678 - this is now leaving the reg with the part of the field clear (0's)
4 val << 16= 0x00AB0000 - value moved into place
5.& mask = 0x00AB0000  extra bits trimmed, none here
6.OR = 0x12AB5678 - the field was 0's, with or - it turns on to 1 if the value to insert had there a 1, and stays 0 else,

It is needed to clear the field first becuse I can only turn on\off bits, not switch them to a specific value when I have no clue what are they now/



## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
Because of integer promotion, uint8_t operands become int in arithmetic, so the expression is an int. Returning it as uint8_t is an implicit narrowing, and -Wconversion warns about it. It does not “obviously” fit: x << 4 produces bits above bit 7. The fix is to mask the nibble first and add an explicit (uint8_t) cast to show that the narrowing is intentional.
