# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:
I build the number digit by digit with `magnitude = magnitude * 10 + digit`, and after every step I check whether it is already above `PARSE_VALUE_MAX` (1,000,000). As soon as it is, I set a "too big" flag and stop adding digits, but I keep reading the rest of the characters to check the syntax. Before each step the magnitude is at most 1,000,000, so the largest value it can ever reach is 1,000,000 * 10 + 9 = 10,000,009, far below the `int32_t` limit, even for a 30-digit input. `int64_t` only moves the problem: a line with 20 or more digits would overflow an `int64_t` too, and signed overflow is undefined behaviour. The input comes from outside, so its length is not under our control. Also, on a small microcontroller 64-bit math is slow and adds code. The real fix is to stop accumulating once the limit is passed.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
The caller may use `out` as "the last valid command" and keep it when parsing fails. Suppose `out` holds `MOTOR_SPD=100` and the next line is `MOTOR_SPD=99999999` (out of range). If the parser had already copied the key and written part of the value before detecting the error, `out` could contain a mix of the old and new command, for example the right key with a wrong value. If any code then applies `out` to the hardware, the motor could jump to an unexpected speed. In my code all checks happen first, using only local variables, and `out->key` and `out->value` are written only in the last lines, after everything has passed.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:
The parser has no control over what arrives on the serial line. A wrong baud rate produces random bytes, electrical noise can flip characters, a person can make typos, and a connected host can be buggy or malicious and send a very long line on purpose to overflow a buffer. If the parser trusts its input, a long key can write past the 11-byte `key` array, and a long number can overflow an integer. "Almost right" input can also be accepted and apply a wrong setting to real hardware. A strict parser that checks every character, every length and every range, and rejects anything that does not match the grammar exactly, keeps the device safe whatever arrives.