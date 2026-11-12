# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
The type of the expression a + b is int, and its value is 300. When I store it in a uint8_t, the value is too large, so only the lowest 8 bits are kept. As a result, the value changes to 44.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
The size of int, short, and char can vary between different systems. In embedded programming, we need to know the exact size of each variable because we work directly with hardware and memory.

For example, if a hardware device sends a 16-bit value, using int might cause unexpected results because its size can differ between systems. Using int16_t ensures that the variable is always exactly 16 bits.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
In unsigned, overflow wraps around to a small value in a predictable way. In signed, overflow causes undefined behaviour, which means the program may produce unexpected results.

In my sat_add_i16 function, I check whether the addition would exceed the maximum or minimum value before performing it. 

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
The result is -5, but the variable cannot store a negative value because it is unsigned. Therefore, the value wraps around to a high number (251). Instead of getting a low battery level, we get a very high value!

With saturation, we check if 10 is greater than level. If it is, we return 0 instead of allowing the value to wrap around.
