# Task 02 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`? What happens when you store it back into a `uint8_t`?

Answer:
Because of integer promotion, both `uint8_t` operands are converted to `int` before the addition, so `a + b` is an `int` with the value 300. Nothing is lost at that point. When 300 is stored back into a `uint8_t`, it is converted modulo 256, so the variable gets 300 - 256 = 44. The wrap happens at the assignment, not at the addition.

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `char`, `short`, `int`? Give a concrete case where plain `int` would surprise you.

Answer:
The size of `char`, `short` and `int` depends on the compiler and the CPU, and even whether `char` is signed is implementation-defined. `uint8_t`, `int16_t` and `uint32_t` have exactly the stated width on every platform, which matters when the value has to match a hardware register, a packet format or a file layout. Example: on an 8-bit or 16-bit microcontroller (like AVR) `int` is only 16 bits, so `int x = 40000;` or `int ms = 1000 * 60;` overflows, while the same code works on a PC where `int` is 32 bits.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow is undefined behaviour. Why is that difference dangerous, and how does your `sat_add_i16` avoid ever overflowing?

Answer:
With unsigned types you get a wrong but predictable result. With signed types the compiler is allowed to assume overflow never happens, so it may optimize away checks such as `if (a + b < a)`, and the program can behave differently between compilers or optimization levels. The bug can stay hidden in testing and appear only in the field. My `sat_add_i16` converts both values to `int32_t` before adding. Any sum of two `int16_t` values is between -65536 and 65534, which always fits in `int32_t`, so the addition itself can never overflow. Only after comparing with `INT16_MIN` and `INT16_MAX` is the result converted back, and by then it is guaranteed to fit.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level - 10` to add a safety margin, and the result is stored in a `uint8_t`. What goes wrong when `level` is 5, and why does saturation fix it?

Answer:
`5 - 10` is -5, and storing -5 in a `uint8_t` wraps it to 256 - 5 = 251. An almost empty battery suddenly reads as almost full, so the device might skip a shutdown or a low-battery warning. With saturating subtraction, any result below 0 is clamped to 0, so the level reads as empty. That is the safe and correct answer.