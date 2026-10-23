# Tutor notes: task 12, Endianness

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Byte order
- Shifts and masks to split and join bytes
- Integer promotion and `0xFF << 24`
- Alignment
- Inspecting an object's bytes through `unsigned char *`

## Common mistakes and the guiding question for each

- **`p[3] << 24` without a cast: UBSan reports a signed shift overflow.**  
  Ask: *What type is `p[3]` after promotion? Does 0xFF000000 fit in it?*
- **Casting `p` to `uint32_t *`.**  
  Ask: *What if `p` points to an odd address? And whose byte order does that read use?*
- **Writing with `p[1] = v >> 8` for a `uint32_t` without masking.**  
  Ask: *Which bits of `v` end up in that byte?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: in big-endian, which byte is the most significant?
2. Concept: a value is the sum of its bytes times 256^position.
3. Point to the shift that happens on a promoted `int`.
4. Pseudo-code: be32 = b0<<24 | b1<<16 | b2<<8 | b3 (each widened to uint32_t first).
5. Example on a different problem: packing an RGB colour into a `uint32_t`.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. Read the bytes `12 34 56 78` as big- and little-endian out loud.
2. Why does your code not care about the host's byte order?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-12`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Why is `*(uint32_t *)p` wrong? Two reasons.
2. Explain the UBSan report you get without the `uint32_t` cast.
3. How does `host_is_little_endian` work?
