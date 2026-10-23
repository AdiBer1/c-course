# Tutor notes: task 15, Button debounce state machine

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- State machines driven by ticks
- Counting consecutive samples
- Instance state in a struct (no globals)
- Events vs. states

## Common mistakes and the guiding question for each

- **Not resetting the counter when the input agrees with the stable state.**  
  Ask: *After 3 "pressed" ticks and 1 "released" tick, what is your counter?*
- **Reporting the event every tick while held.**  
  Ask: *When exactly should PRESS be returned: on one tick, or on every tick?*
- **Using `static` variables inside `debounce_update`.**  
  Ask: *What happens with two buttons?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to draw the two states and label every arrow with its condition.
2. Concept: the counter measures how long the input has disagreed with the stable state.
3. Point to the branch where the input equals the stable state.
4. Pseudo-code: if raw == stable: counter = 0; else if ++counter >= threshold: flip, counter = 0, return event.
5. Example on a different problem: detecting that a sensor value stayed above a limit for 10 samples.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. With threshold 5, on which tick exactly does PRESS come?
2. Why is a 3-tick glitch ignored by your code?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-15`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Draw your state machine.
2. Why not just `delay(20)`?
3. Why is the state in a struct instead of `static` variables?
