# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
I check the value before multiplying by 10 or adding the next digit. If it would exceed the allowed limit, I mark it as out of range without doing the operation. Using int64_t is not enough because a very long input can overflow that type too.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
The output must stay unchanged if parsing fails. Otherwise, the device might use a partially parsed command and apply incorrect settings to the hardware.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
Input from a serial port is not always valid or safe. It can contain unexpected characters, very long strings or malicious data. The parser must validate everything before accepting a command.
