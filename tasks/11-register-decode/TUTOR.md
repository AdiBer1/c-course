# Tutor notes: task 11, Decoding a status register

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Bit fields by hand: shift then mask
- Two's complement and sign extension of an n-bit field
- `volatile` and memory-mapped registers
- The pointer-to-address idea: `*(volatile uint32_t *)0x40021000u`

## Common mistakes and the guiding question for each

- **Treating the 12-bit temperature as unsigned.**  
  Ask: *What does 0xFFF mean in 12-bit two's complement?*
- **Masking before shifting with the wrong mask (or vice versa).**  
  Ask: *After `reg >> 16`, which bits hold the temperature?*
- **Reading `*reg` once per field in `status_read`.**  
  Ask: *If the hardware changes the register between your reads, what do you get?*
- **Encoding an invalid mode as 4.**  
  Ask: *What does the header say an invalid mode encodes to?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to draw the 32 bits and mark each field.
2. Concept: `(reg >> pos) & mask` extracts any field.
3. Point to the temperature line and ask what happens when bit 11 of the field is set.
4. Pseudo-code: raw = field; if raw & 0x800: value = raw - 4096.
5. Example on a different problem: decoding an 8-bit RGB332 colour into r, g, b.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. Why does `status_read` take a `const volatile` pointer?
2. Decode 0xF60 by hand into a temperature.

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-11`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Show your sign extension and explain why it works.
2. What could the compiler do without `volatile`?
3. What does `*(volatile uint32_t *)0x40021000u` mean, and why does it crash on a PC?
