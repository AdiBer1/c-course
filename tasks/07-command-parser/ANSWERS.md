# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
Before each step I check whether `mag * 10 + digit` still fits under the
limit, using `mag > (limit - digit) / 10`. The check happens before the
multiplication, so the number itself never overflows. Using `int64_t`
is not a complete answer, because a line can contain more digits than
even 64 bits can hold ("LED=99999999999999999999"), so I would still
need an overflow check.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
If the value is inserted at the beginning, a situation may arise where only half the value is written; subsequently, an error is discovered, leaving the `out` parameter filled with garbage data.
It is best to avoid this, to prevent the accidental use of garbage data, which would lead to an incorrect result.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
Input comes from an external source; therefore, instead of valid data, it could consist of noise or even malicious code. If we assume the input is valid, our code could be compromised, or bugs could be introduced.
