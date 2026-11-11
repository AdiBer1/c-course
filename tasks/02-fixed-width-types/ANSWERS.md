# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
Because of integer promotion, both operands are converted to `int` before the
addition, so `a + b` has type `int` and value 300. Nothing overflows at this
point. When 300 is stored back into a `uint8_t`, it is converted modulo 256
(this is well defined for unsigned types), so the stored value is 300 - 256 = 44.
The compiler may also warn about the narrowing conversion (`-Wconversion`).

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
The C standard only gives minimum sizes for `char`, `short` and `int`, and the
real size depends on the compiler and the CPU. Whether plain `char` is signed is
also implementation-defined. In embedded code we often need an exact width:
hardware registers, communication protocols, and memory-limited buffers.
The `stdint.h` types guarantee "exactly N bits" on every compiler.
Example: on AVR (Arduino Uno) `int` is only 16 bits. Code like
`int ms = 60 * 1000;` works on a PC (32-bit int), but on AVR 60000 does not fit
in a 16-bit `int`. That is signed overflow, and you get a negative or
unpredictable value. With `uint32_t ms = 60UL * 1000UL;` it is correct everywhere.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
With unsigned types you always know the result (modulo 2^N). With signed types
the compiler is allowed to assume that overflow never happens, and it optimizes
based on that assumption. For example, it can delete an overflow check like
`if (a + b < a)`, because by its logic that check can never be true. So the
program does not just produce a "wrong number"; its behaviour can be anything,
and it can change between compilers or optimization levels. The bug is
dangerous because code that "works" on one setup can break on another.
My `sat_add_i16` converts both values to `int32_t` before adding. The sum of
two `int16_t` values is in the range [-65536, 65534], which always fits in
`int32_t`, so the addition can never overflow. Only after that do I compare
against `INT16_MAX` and `INT16_MIN`, and I cast back to `int16_t` only when the
value is already in range.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
`5 - 10` is computed as `int` and gives -5. Storing -5 in a `uint8_t` wraps
around modulo 256, so the value becomes 251. A battery that is almost empty
suddenly looks almost full. The system will not warn and will not shut down
safely, and the device may turn off unexpectedly or be damaged.
With saturating subtraction (`sat_sub_u8(level, 10)`), any result below 0 is
clamped to 0. The battery is reported as empty, which is the safe and correct
reaction.
