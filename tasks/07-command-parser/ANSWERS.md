# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
Before adding each digit, the parser checks if the next calculation will exceed the allowed limit. If it will, it marks the value as out of range without performing the calculation. Using int64_t is not a complete solution because a very long input can also overflow it.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
The output must stay unchanged so the program will not use incomplete or incorrect data. For example, a device could receive an invalid motor speed command and accidentally use a partially updated value. This could cause unexpected behavior.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
Because input from a serial port is not always correct or safe. A user or another device can send unexpected characters, very long lines, or invalid values. The parser must check everything to prevent memory problems and unexpected behavior.
