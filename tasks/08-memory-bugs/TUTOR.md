# Tutor notes: task 08, Hunting memory bugs

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Static vs. stack lifetime; dangling pointers
- Out-of-bounds reads and writes; off-by-one bounds
- Reading ASan/UBSan reports
- Integer type too small for a sum
- Division by zero

## Common mistakes and the guiding question for each

- **Silencing a report instead of fixing the cause (e.g. making the array bigger).**  
  Ask: *Why was index 8 used at all? What does the header promise about capacity?*
- **Fixing `log_label` by returning a string literal and ignoring why the original was wrong.**  
  Ask: *Where did `buf` live, and when did it stop existing?*
- **Treating the average bug as a memory bug.**  
  Ask: *What is the largest possible sum of 8 readings? Does it fit in the sum's type?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to read the first report aloud: what kind, which line, which function called it?
2. Concept for the kind of report she is stuck on (bounds, lifetime, overflow).
3. Point to the exact line from the report and ask what values the index can take there.
4. Pseudo-code of the correct condition (e.g. `if count >= CAPACITY: return false`).
5. Example on a different problem: a function that returns a pointer to a local array.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. For each of the 6 bugs: what was the symptom, which tool showed it, what was the root cause?
2. Why did the `log_label` bug sometimes "work" even without the fix?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-08`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Pick any bug: show the report or failing test, then your fix, and explain the root cause.
2. Explain stack vs. static lifetime using `log_label`.
3. Why does embedded code avoid `malloc`?
