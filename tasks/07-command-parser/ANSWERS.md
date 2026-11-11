# Task 07 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. How does your parser detect that a value is out of range without ever overflowing `int32_t`? Why is "just use `int64_t`" not a complete answer?

Answer:

I build the number one digit at a time (value * 10 + digit). After each digit I check whether the value is greater than PARSE_VALUE_MAX (1,000,000). If it is, I stop immediately and return PARSE_ERR_RANGE.
This is safe because before each step the value is at most 1,000,000. After multiplying by 10 and adding a digit, it is at most about 10,000,009, which is far below the int32_t limit (about 2.1 billion). So the value can never overflow.
"Just use int64_t" is not a complete answer because it only moves the limit. int64_t also overflows, after about 19 digits. An input with 20 or more digits, like 99999999999999999999, would overflow it too.

## Q2. Why must `*out` stay unchanged on every error? Describe what could go wrong on a device if a half-parsed command were left in `out`.

Answer:
I write to out only at the very end, after every check has passed.
If I wrote part of the result and then found an error, out would hold a mix of new and old data, for example a new key with an old value. A device using out could then drive the hardware with a command nobody sent, such as running a motor at the wrong speed.
This way, the caller can rely on one simple rule: if the function returns an error, out did not change.

## Q3. The input line comes from a serial port, typed by a human or sent by another device. Why should a parser assume the input can be anything, including very long or malicious lines?

Answer:


The input comes from outside, over a serial port. A person can make a typing mistake, electrical noise can corrupt characters, and someone can send a malicious line on purpose.
If the parser trusts its input, a line that is too long can write past the end of an array and corrupt memory, and a huge number can overflow into a wrong value.
That is why the parser checks every character, the key length and the value range, and rejects any line that is not exactly valid.