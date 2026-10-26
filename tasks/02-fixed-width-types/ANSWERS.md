# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer: 
the type is int, but then when it is stored into a uint8_t it cuts all the left bits - MSBs and leaves only the 8 LSBs, so it was 300 , and after the cut it will be 44 

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
Because they are fix-sized, the size of an int can change by compiler (16 or 32 bits) and so the other types like char, short or so, the uintX_t are fix sized, and this is important for embedded systems that we want to know exactly what is with every bit, not only with the number, exaple- i expect int to be 32 bits as regular, then I have int a=32767;a++; this will give me a negative answer just because the compiler gave the int 16 bit only!
## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
Unsigned - the answer will be % 2^bits, signed - it ffalls back to the minus, the first bit changes it to minus instead of what was expected - increasing the number, the sat_add_i16 avoids the overflowing by saving the result into a 32 bits variable, (16 bit + 16 bits are less then 32 bits) so there is no overflow, just we check if it would cause an overflow it was saved in a 16 bit, and return the currct answer accurding to the ifs

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it? 

Answer: 
Level is 5, then the code does level-10; the answer should be minus 5, but its unsigned! so it will wrap around to a big number (251) and will presend a high battery value instead of no battery!Saturation prevents this by returning 0 if it is about to get to the minus numbers

