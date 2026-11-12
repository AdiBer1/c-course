# Task 10 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `#define BIT(n) (1 << (n))` wrong for n = 31, while `((uint32_t)1u << (n))` is fine? What did UBSan or the compiler tell you?

Answer:
A normal `1` is a signed int, and bit 31 is the sign bit. If you shift a 1 into it, the result doesn't fit in an int, so it's undefined behavior. With `(uint32_t)1u` the number has no sign, so shifting by 31 is totally fine and you get 0x80000000. I used the unsigned version from the start, so I never got the error. But UBSan would say something like "left shift of 1 by 31 places cannot be represented in type int".

## Q2. What goes wrong with `BIT_SET(reg, i++)` (or with an unparenthesised macro like `#define BIT(n) 1u << n` used as `BIT(1 + 2)`)? Why do functions not have these problems?

Answer:
A macro just pastes the text you give it. So if the argument shows up twice in the macro, `i++` runs twice and `i` goes up by 2 instead of 1. Missing parentheses cause a similar mess: `BIT(1 + 2)` turns into `1u << 1 + 2`, and the math happens in the wrong order. A function doesn't have this problem, because it works out each argument once, before the call, and then just uses that value.

## Q3. Explain `FIELD_SET` step by step for `FIELD_SET(0x12345678, 16, 8, 0xAB)`. Why must it first clear the field?

Answer:
First the macro makes a mask: 8 ones starting at bit 16, which is 0x00FF0000.
Then it clears that spot: 0x12345678 & ~0x00FF0000 gives 0x12005678.
Then it moves the new value into place: 0xAB << 16 gives 0x00AB0000.
Last, it puts them together with OR: 0x12005678 | 0x00AB0000 gives 0x12AB5678.
It has to clear first because OR can only turn bits on, not off. If you skip that, the old bits (0x34) stay on and mix with 0xAB, so you get the wrong number.
## Q4. `-Wconversion` warned about `return (x << 4) | (x >> 4);` for a `uint8_t x`. Why, if the value "obviously" fits?

Answer:
Before doing any math, C turns `x` from a `uint8_t` into an `int`. So `x << 4` is an int and can have bits past bit 7, and the whole result is an int too. When you return it as a `uint8_t`, the extra bits get cut off. `-Wconversion` warns about any cut like that it can't be sure about. So I made it clear on purpose: I do the math in a `uint32_t`, keep only the low 8 bits with `& 0xFFu`, and cast to `uint8_t`.

