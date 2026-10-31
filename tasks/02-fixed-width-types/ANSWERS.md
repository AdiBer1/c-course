# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
The type of a + b is int, not uint8_t, and its value is 300. Before the addition, both uint8_t operands are promoted to int (integer promotion), so the addition happens in int and does not overflow. When 300 is stored back into a uint8_t, it is converted modulo 256, so the stored value is 44 (300 − 256). This conversion is well defined for unsigned types, but the information is lost silently.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
The C standard does not fix the size of char, short and int; it only gives minimum sizes. On a PC int is 32 bits, but on many small microcontrollers (for example AVR or MSP430) it is 16 bits. Even whether plain char is signed or unsigned depends on the compiler. In embedded code we often need an exact size, for example to match a hardware register or a communication protocol, so we use uint8_t, int16_t, uint32_t, which have the same size on every compiler.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
Unsigned overflow always wraps in a predictable way, so many programmers assume signed overflow also just wraps. It does not: signed overflow is undefined behaviour, and the compiler is allowed to assume it never happens. When optimizing, it may remove checks like if (a + b < a) because, by that assumption, the condition can never be true. So code that seems to work can break at a different optimization level or on a different compiler, and checking for overflow after doing the operation is already too late. My sat_add_i16 avoids this by converting both values to int32_t before adding. The sum of two int16_t values is always between −65536 and 65534, which always fits in int32_t, so the addition never overflows. Only then do I compare the result with INT16_MAX and INT16_MIN, and cast it back to int16_t only when it is in range.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
 level is promoted to int, so level - 10 with level == 5 gives −5. A uint8_t cannot hold a negative value, so when −5 is stored, it wraps to 251 (256 − 5). The battery, which is almost empty, now looks almost full, so the device does not warn the user or shut down safely, and may suddenly die. With saturation (sat_sub_u8(level, 10)), the result is clamped to 0, so an almost-empty battery is reported as empty, which is the safe and correct answer.
 
