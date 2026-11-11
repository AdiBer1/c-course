# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
The parser checks if the current accumulated value will exceed PARSE_VALUE_MAX or fall below PARSE_VALUE_MIN before multiplying by 10 and adding the next digit (e.g., checking if accum > (PARSE_VALUE_MAX - digit) / 10). "Just use int64_t" is not a complete answer because if an input line contains an extremely long sequence of digits (like a string of 50 digits), storing or casting it would still eventually overflow any fixed-size integer type, or require arbitrary-precision arithmetic. Checking boundaries step-by-step prevents any overflow during accumulation

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:*out must remain unchanged to guarantee atomicity and fail-safe behavior. If a command is only half-parsed and partial values are written to *out, the system might execute an unintended or corrupted state (for example, updating a motor speed parameter with a partially parsed number while keeping the old variable name, or vice versa), which could lead to erratic hardware behavior, safety hazards, or device malfunctions in an embedded environment.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
A parser running on an embedded device is exposed to an untrusted external environment where noise, faulty hardware, communication glitches, or malicious actors can transmit arbitrary data. Assuming input is well-behaved can lead to buffer overflows, memory corruption, or unexpected crashes. A robust parser must defensively validate every single byte to ensure the system remains stable and secure against malformed or malicious payloads.
