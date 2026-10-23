# Tutor notes: task 17, Q16.16 fixed-point arithmetic

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Q formats: value = raw / 2^16
- Why multiplication needs a 64-bit intermediate and a rescale
- Rounding with the remainder
- Saturation

## Common mistakes and the guiding question for each

- **`(a * b) >> 16` in 32 bits.**  
  Ask: *How many bits can `a * b` need?*
- **`x << 16` for negative `x`.**  
  Ask: *What does the C standard say about left-shifting a negative value?*
- **Rounding with `+ 0x8000` for negative results.**  
  Ask: *Which direction does C round a negative quotient, and which direction do you need?*
- **Forgetting saturation after the 64-bit computation.**  
  Ask: *What happens when you cast 2^40 to `int32_t`?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: if both operands are value × 65536, what is the product made of?
2. Concept: compute exactly in 64 bits, round, then clamp.
3. Point to the line that overflows or rounds the wrong way.
4. Pseudo-code for mul: p = (int64)a * b; r = round_div(p, 65536); return clamp(r).
5. Example on a different problem: multiplying two percentages stored as 0..10000.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. Why does `q16_mul` divide by 65536 and not by 65536²?
2. What does your `q16_div(Q16_MIN, Q(-1))` return, and why?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-17`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Explain the 64-bit intermediate in `q16_mul`.
2. Show your rounding helper with a negative example.
3. Why saturate in a PID controller?
