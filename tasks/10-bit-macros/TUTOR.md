# Tutor notes: task 10, Bit manipulation macros

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Bitwise `& | ^ ~ << >>`
- Masks and fields
- Unsigned constants (`1u`) and why signed shifts are dangerous
- Macro pitfalls: parentheses, multiple evaluation
- Integer promotion of `uint8_t` in shifts

## Common mistakes and the guiding question for each

- **`#define BIT(n) (1 << (n))`: signed int, UB at n = 31.**  
  Ask: *What is the type of the literal `1`? What happens when you shift it into bit 31?*
- **Missing parentheses: `~BIT(3)` or `BIT(1 + 2)` breaks.**  
  Ask: *Expand your macro by hand for `~BIT(3)`. What does the compiler see?*
- **`FIELD_MASK(0, 32)` computing `1u << 32`.**  
  Ask: *Is shifting a 32-bit value by 32 defined? Can you build the mask by shifting right instead?*
- **`FIELD_SET` that ORs without clearing first.**  
  Ask: *What happens to the bits that were already in the field?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: on paper, which bits do you need to change, and with which operator?
2. Concept: AND with a mask keeps bits, OR sets, XOR flips, AND with ~mask clears.
3. Point to the macro whose expansion is wrong for the failing test's input.
4. Pseudo-code for FIELD_SET: (reg & ~mask) | ((val << pos) & mask).
5. Example on a different problem: macros for the high and low byte of a 16-bit value.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. Expand `~BIT(3)` with your macro by hand.
2. Why is `FIELD_MASK(0, 32)` tricky, and how does yours handle it?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-10`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Explain `FIELD_SET(0x12345678, 16, 8, 0xAB)` step by step.
2. What goes wrong with `BIT_SET(reg, i++)`?
3. Why `1u` and not `1`?
