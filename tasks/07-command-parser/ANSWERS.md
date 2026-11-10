# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
Instead of multiplying first and checking afterward, I check if the next digit would cause an overflow before doing the math, using value > (MAX - digit) / 10. If that condition is true, I stop immediately and return a range error. Using int64_t wouldn't actually solve this—it just pushes the limit further away. A 20-digit number like X=99999999999999999999 would still overflow int64_t, plus 64-bit arithmetic adds unnecessary overhead on small microcontrollers.


## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
If the function fails partway through, the caller might still inspect or reuse out by mistake. Leaving out in a half-updated state—like a new key paired with an old value—could cause the system to apply a setting that was never actually sent. For example, if a motor command fails to parse its new speed but the key was already overwritten, the motor might end up running at whatever speed was left in out from the previous command.


## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
Because input comes from the outside world, it can be corrupted by line noise, a mismatched baud rate, a simple typo, or even someone intentionally sending malformed data. If the parser assumes the input is always well-formed and skips bounds checking, a line that is too long can easily write past the end of a buffer (like writing past key[11]), leading to memory corruption or a crash.

