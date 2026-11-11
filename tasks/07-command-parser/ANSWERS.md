# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
Instead of calculating the full value and checking after an overflow occurs (which is undefined behavior in C), the parser checks for potential overflow before performing each multiplication and addition. Relying on `int64_t` is not a complete answer because small embedded microcontrollers may lack native 64-bit hardware support, leading to inefficient code, and even a 64-bit integer can still overflow if given a sufficiently long input string.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
Keeping `*out` unchanged on error prevents atomic state corruption. If a parser updates fields of `*out` partially (for example, setting the key to `MOTOR_SPD` before detecting that the value is out of range), another background task or interrupt handler reading `*out` might execute an unvalidated or half-parsed command, causing dangerous physical side effects like driving hardware out of safe limits.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
Serial communications are vulnerable to hardware noise, line corruptions, misconfigured baud rates, and malicious input attacks. A robust embedded parser must treat all external input as completely untrusted to prevent buffer overflows, stack corruption, or unexpected application crashes when receiving corrupted or excessively long data streams.
