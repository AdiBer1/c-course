# Tutor notes: task 02, Fixed-width types and saturation

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- `stdint.h` fixed-width types and their limits
- Integer promotion: `uint8_t + uint8_t` is an `int`
- Unsigned wrap is defined; signed overflow is undefined behaviour

## Common mistakes and the guiding question for each

- **`if (a + b > 255)` after storing the sum in a `uint8_t` (already wrapped).**  
  Ask: *At which point in your code has the value already been cut to 8 bits?*
- **Testing `a - b < 0` on unsigned values (always false).**  
  Ask: *Can an unsigned value ever be negative? So what does that comparison really test?*
- **Forgetting that `0 - INT16_MIN` does not fit in `int16_t`.**  
  Ask: *What is the largest `int16_t`? And what is `-(-32768)`?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: what is the exact mathematical result, and where could you store it without losing anything?
2. Concept: compute in a wider type, compare with the limits, then convert back.
3. Point to the line where the sum is stored in the narrow type.
4. Pseudo-code: wide = a + b (in int32_t); if wide > MAX return MAX; if wide < MIN return MIN; return wide.
5. Example on a different problem: clamping a percentage to 0..100.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What is the type of `a + b` when both are `uint8_t`?
2. Why can't `sat_add_i16` overflow in your implementation, for any two inputs?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-02`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Explain integer promotion using your `sat_add_u8`.
2. Which line of yours would be undefined behaviour if you used `int16_t` for the intermediate result?
3. Why does embedded code prefer `uint16_t` over `unsigned short`?
