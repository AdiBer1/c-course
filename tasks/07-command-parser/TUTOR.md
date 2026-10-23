# Tutor notes: task 07, Command-line parser

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Walking a string with an index
- Validating input character by character
- Overflow-safe digit accumulation
- Structs with fixed-size char arrays
- Error codes with a defined precedence

## Common mistakes and the guiding question for each

- **Writing the key into `out->key` while still scanning.**  
  Ask: *If the value later turns out invalid, what is left in `out`?*
- **`value = value * 10 + digit` overflowing on long inputs.**  
  Ask: *What happens to an `int32_t` after 11 digits of 9?*
- **Accepting `\r` alone or `\n\n` at the end.**  
  Ask: *Exactly which line endings does the header allow?*
- **Checking range before finishing the syntax (`X=99999999x` gives RANGE).**  
  Ask: *Which check comes first in the header's list?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to write the steps of the error list as a numbered plan before coding.
2. Concept: first find the end of the meaningful text, then the key, then the value.
3. Point to the first check that runs in the wrong order, using the failing test's input.
4. Pseudo-code: strip ending → scan key chars until '=' → validate length → parse sign and digits with a limit → copy to out.
5. Example on a different problem: parsing an `HH:MM` time string with error codes.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. Give me an input where two rules are broken. Which error does your code return and why?
2. How does your code stop a 30-digit number from overflowing?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-07`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Show where your code guarantees `*out` stays unchanged on error.
2. Walk through `parse_command("LED=99999999x", &c)`.
3. Why is a strict parser safer on a device than a forgiving one?
