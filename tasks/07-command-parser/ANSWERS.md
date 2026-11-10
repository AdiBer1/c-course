# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
For each digit I compute `magnitude * 10 + digit`. Before the multiplication `magnitude` is at most 1,000,000, because as soon as it exceeds the limit I set a flag and stop accumulating. So the result is at most 10,000,009, which fits in `int32_t` and can never overflow. I keep checking the remaining characters as digits, so a line like `LED=99999999x` still returns BAD_VALUE rather than RANGE, as the header's check order requires. Just using `int64_t` is not a complete answer: it is slow and costly on small 8- or 16-bit chips, and it only moves the problem, because a string of 30 digits overflows even 64 bits. The accumulation still has to be stopped in time.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
The caller may still use `out` after an error, for example if it did not check the status. If a half-parsed command were left there, such as a new key with an old value or a partial key, the device could apply a wrong setting to real hardware: for example a MOTOR_SPD command with the value of a different command, making the motor run at the wrong speed. If `out` is left untouched, it still holds the last valid command, which is much safer.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
Input from outside can be anything: line noise, a wrong baud rate, a faulty device on the other end, or a deliberate attack. A parser that trusts its input can suffer a buffer overflow from an overly long line, corrupt memory, and in the worst case let an attacker run their own code. Even "almost right" input that gets accepted can apply wrong settings. So the parser checks every character, limits lengths, and rejects anything that does not match the grammar exactly.