# Tutor notes: task 03, Integer-only math with correct rounding

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Integer division truncates toward zero
- Rounding by adding half the divisor (and the sign problem)
- Order of operations: multiply before divide
- Choosing intermediate widths

## Common mistakes and the guiding question for each

- **`c10 * 9 / 5` without rounding: off by one on many inputs.**  
  Ask: *What does C do with the remainder of `9 / 5`? What should happen to it?*
- **Adding half the divisor also for negative numbers.**  
  Ask: *Try your formula by hand on -1.8. Which way does C's division move a negative result?*
- **`9 / 5 * c10`: integer division first.**  
  Ask: *What is the value of `9 / 5` in C?*
- **Computing `raw * 3300` in `uint16_t`.**  
  Ask: *What is the largest value `raw * 3300` can reach? Does it fit in 16 bits?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask what 1.8 rounds to, and what -1.8 rounds to.
2. Concept: integer division throws away the remainder; rounding means looking at it first.
3. Point to the division line that ignores the sign.
4. Pseudo-code: if numerator >= 0: (n + d/2) / d, else (n - d/2) / d.
5. Example on a different problem: average of two integers rounded to nearest.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What does your `c10_to_f10(-1)` return and why?
2. Where in `adc_to_mv` would a 16-bit `int` overflow, and how did you prevent it?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-03`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Show me the rounding for a negative number step by step.
2. Why multiply before dividing?
3. What would break on an AVR (16-bit int) in a naive `adc_to_mv`?
