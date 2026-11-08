# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
I check whether the next digit would exceed the allowed range before performing multiplication and addition (old + new*10). This prevents signed integer overflow, which causes undefined behavior in C. Using int64_t is not a complete solution because embedded systems may not support it efficiently, and even int64_t has a finite range

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
The device then might try to run an incomplete command, and break things later on, we expect to ge or - a full command, or NULL, not somthing that is not one of those 2/
## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
External data can never be trusted, this is a basic rule, because the user may typo, milicsios type errors, noise and the same for devices