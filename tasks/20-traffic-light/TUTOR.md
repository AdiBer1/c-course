# Tutor notes: task 20, Non-blocking traffic light

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Non-blocking design: poll, act, return
- Wrap-safe unsigned time arithmetic
- State machines with timed transitions
- Mock clocks make time deterministic in tests

## Common mistakes and the guiding question for each

- **`while (now - start < duration) {}`: the test hangs (5 s timeout).**  
  Ask: *With the mock clock, who moves time forward while your loop runs?*
- **`now >= start + duration`.**  
  Ask: *What is `start + duration` when start is 2000 ms before the wrap?*
- **Several phase changes in one update after a long gap.**  
  Ask: *What does the header say about the number of changes per call?*
- **Clearing the pedestrian request at the wrong moment.**  
  Ask: *When exactly is the request "served"?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to list, for each phase, the condition to leave it and the lamps that are on.
2. Concept: compute `elapsed` once per update with unsigned subtraction.
3. Point to the comparison or the phase case that fails the test.
4. Pseudo-code: now = clock(); elapsed = now - start; read button; switch (phase) { case RED: if elapsed >= RED_MS: enter(RED_YELLOW, now) ... }
5. Example on a different problem: an LED that blinks 200 ms on / 800 ms off without delay.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. Why does your time comparison still work when the counter wraps from 0xFFFFFFFF to 0?
2. What would happen to a UART in the same main loop if `tl_update` waited?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-20`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Show the wrap-around case with numbers.
2. Why does the new phase start at "now"?
3. What exactly changes when this runs on a real board?
