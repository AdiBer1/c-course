# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
The parser stops increasing the number once it reaches the maximum limit. Using int64_t is not enough because someone could send a string with a million digits,  that could overflow any integer size.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
The update must be "all or nothing". If we change *out during an error, the physical device might receive a broken command and act dangerously, like turning on a motor at the wrong speed.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
Cables and serial ports can send garbage data because of electrical noise or bad connections. Also, people might send bad data on purpose to crash the system.
