# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
The expression a + b has type int because of integer promotion in C, and its value is 300. When we store it back into a uint8_t, the value wraps around to 44 because uint8_t can only hold values from 0 to 255.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
Fixed-width types guarantee the exact number of bits, regardless of the compiler or platform. This is important in embedded systems because hardware registers, communication protocols and sensor data often require specific data sizes. For example, an int is not guaranteed to be 16 bits on every system, so using it to represent a 16-bit hardware value could cause unexpected results.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
Signed overflow is dangerous because the C standard does not define what should happen, so the compiler may produce unexpected results. In sat_add_i16, both values are converted to int32_t before addition. The result is checked against INT16_MAX and INT16_MIN, and only then converted back to int16_t. This prevents signed overflow and keeps the result within the valid range.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
When level is 5, subtracting 10 gives -5. When this result is stored in a uint8_t, it wraps around to 251 instead of representing a negative value. This is dangerous because the battery level appears almost full instead of low. Saturation fixes this by returning 0 whenever the subtraction would produce a negative result.
