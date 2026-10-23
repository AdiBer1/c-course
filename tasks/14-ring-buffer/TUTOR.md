# Tutor notes: task 14, Ring buffer

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Circular indexing and wrap-around
- Distinguishing full from empty
- Caller-owned storage
- Status codes for full/empty

## Common mistakes and the guiding question for each

- **Wrap with `if (index > size)` instead of `== size`.**  
  Ask: *What is the largest valid index into an array of `size` elements?*
- **Using only head == tail to mean empty, so a full buffer looks empty.**  
  Ask: *What do head and tail look like after exactly `size` pushes?*
- **Writing `*out` on an empty pop.**  
  Ask: *What does the header promise about `out` on `RB_ERR_EMPTY`?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to draw a 4-slot buffer and push/pop on paper, tracking head, tail and count.
2. Concept: each index moves forward by one and jumps back to 0 at the end.
3. Point to the wrap or full/empty condition that the failing test exercises.
4. Pseudo-code: push: if count == size: FULL; buf[head] = b; head = next(head); count++.
5. Example on a different problem: a circular list of the last 5 commands.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. After 4 pushes and 2 pops in a 4-byte buffer, where are head and tail?
2. How does your code tell full from empty?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-14`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Walk through a wrap-around on paper.
2. In a UART driver the ISR pushes and the main loop pops: which field is dangerous here and why?
3. Why does the caller provide the storage?
