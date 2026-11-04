# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:

When you add in C, the values of the variables are converted to `int`. When the addition is done in `int`, the type of the result `a + b` is also `int`.
Its value will be 300.
But when the result is converted back into the result variable, which is of type `uint8_t` and whose maximum is 255, the value reaches the maximum, gets cut off and wraps around to the beginning. In the end the value is 44.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
Embedded code uses types with a fixed number of bits instead of the plain types, because on embedded systems each system can have a different size for the types and a different number of bits. On hardware this has to be exact: for example, we write directly to registers, and if there is an overflow we get a wrong value.
That is why we use types whose number of bits is always fixed.
For example, if we add 30000 + 30000 in `int`, the result really is 60000. On my computer this works fine, because there `int` is 32 bits, but on another computer where `int` is 16 bits it overflows the limits of `int` and we get a wrong result.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
It is dangerous because we don't know what result we will get or whether it will be wrong, and it can return incorrect results to our systems. The compiler may decide that overflow never actually happens, and then if we check after the overflow has already happened, we get a wrong value and don't know that it is wrong.
In the function we checked the bounds: whether the result is below the minimum or above the maximum, and if so we limited it to the edge value.
We did the check in `int`, so we got the correct value of the arithmetic operation, and then we checked it against the limits of `int16_t`.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:

When the value is 5, it is too close to the lowest value, and when we compute 5 - 10 the result is below the lowest possible value (0). Then the code reads it as almost the highest battery value (256 - 5), and instead of understanding that the battery is low, it thinks the battery is full, and its behaviour does not match reality.
In the function we wrote, there is first a check, on a variable of type `int`, of whether the result goes beyond the limits, and in this case we would return 0.