# Tutor notes: task 13, CRC-8

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- CRC as polynomial division in shift-register form
- Keeping a value 8 bits wide
- Streaming computation (update per byte)
- Error detection vs. correction

## Common mistakes and the guiding question for each

- **Testing bit 0 instead of bit 7 (that is the reflected variant).**  
  Ask: *Which bit does the header say to look at before shifting?*
- **Shifting first, then testing the top bit.**  
  Ask: *In the algorithm, is the top bit tested before or after the shift?*
- **`crc8_frame_ok` including the CRC byte in its own computation.**  
  Ask: *Which bytes is the last byte the CRC *of*?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to run the algorithm by hand for one byte, `0x01`.
2. Concept: XOR the byte in, then 8 steps of shift-and-maybe-XOR.
3. Point to the line that tests the wrong bit or shifts at the wrong moment.
4. Pseudo-code: exactly the one in the header comment.
5. Example on a different problem: a simple 8-bit sum checksum, and why it misses swapped bytes.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What does your `crc8("123456789", 9)` return, and why is that number famous?
2. Why does `crc8_update` need a cast under `-Wconversion`?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-13`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Which errors does a CRC catch that a sum does not?
2. Table-driven vs. bit-by-bit: when would you use which on an MCU?
3. Run `crc8_update(0x00, 0x80)` on paper.
