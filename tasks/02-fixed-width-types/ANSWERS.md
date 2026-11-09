# Answers - Task 02: Fixed-width types and saturation

## Q1. In `uint8_t a = 200, b = 100;` what is the type and the value of the expression `a + b`?

Answer:
Due to integer promotion rules in C, operands smaller than `int` are promoted to `int` (or `unsigned int`) before any arithmetic operation is performed. Therefore, the type of `a + b` is `int`, and its numerical value is 300 (which exceeds `UINT8_MAX`).

## Q2. Why does embedded code use `uint8_t`, `int16_t`, `uint32_t` instead of `int` or `long`?

Answer:
Standard C types like `int` have sizes that vary depending on the compiler and target architecture. Fixed-width types guarantee exact bit-lengths across all platforms, which is essential in embedded systems for interacting directly with hardware registers, memory-mapped peripherals, and communication protocols.

## Q3. Unsigned overflow wraps around in a defined way, but signed overflow...

Answer:
Unsigned integer overflow is strictly defined by the C standard to wrap around modulo $2^n$. In contrast, signed integer overflow is undefined behavior (UB), meaning the C standard places no requirements on what happens, allowing compilers to make optimizations that can break program logic.

## Q4. A battery-level reading is `uint8_t level` (0–255). The code does `level -= 10` when `level` is 5. What happens without saturation?

Answer:
Without saturation, unsigned wrapping occurs: subtracting 10 from 5 underflows past 0 and wraps around to `251`. This causes a nearly depleted battery to incorrectly register as almost fully charged.
