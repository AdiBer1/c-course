# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
The parser checks each digit before adding it to the number. If the number becomes bigger than the allowed limit, it returns a range error. Using int64_t is not enough because the input can be very long, and even this type has a limit.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
The output should not change if the command is invalid. Otherwise, the device might use incorrect information and do something it should not do. That is why the parser updates out only after all the checks are successful.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
The input does not always come in the correct format. It can contain wrong characters, be too long, or be sent by a device with bad intentions. The parser needs to check everything carefully to avoid errors and problems.
