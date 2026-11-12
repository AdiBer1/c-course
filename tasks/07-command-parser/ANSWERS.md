# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
We check the value digit by digit during conversion. Before multiplying the current total by 10 and adding the new digit, we check if it exceeds the maximum safe limit. Using `int64_t` is not a complete answer because on very small microcontrollers, 64-bit math is slow and expensive, and a malicious user could still input a huge number that overflows even 64 bits.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
If an error happens halfway, keeping `out` unchanged ensures the device does not apply bad or partial settings. If a half-parsed command was left, the device could change hardware parameters incorrectly, leading to unexpected behavior or hardware damage.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
Because serial communication can be noisy, corrupted, or sent by faulty software and malicious users trying to crash the system. A secure parser never trusts input and must validate every character safely.
