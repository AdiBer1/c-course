# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
Before each step `mag * 10 + d` I check `mag > (limit - d) / 10`. If it is
true, the next step would pass the limit, so I stop with PARSE_ERR_RANGE
before doing the math. int64_t only moves the problem: a line with 30 digits
overflows it too. Also 64-bit math is slow on small MCUs.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
The caller may keep using `out` as the current setting. If a bad line left a
new key with an old value (or half a value), the device could apply a wrong
setting, e.g. a wrong motor speed. So I check everything first and write
`out` only at the end.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
The line comes from outside: noise, a wrong baud rate, a bug in the other
device, or an attacker. If the parser trusts it, a long key can overflow the
buffer, and "almost right" input changes real hardware. So every char and
every length is checked.