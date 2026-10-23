# Tutor notes: task 04, Enums and switch

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- `enum` as a named set of integer constants with its own type
- `switch` with fall-through case groups and `default`
- String literals live for the whole program
- Defensive handling of impossible values

## Common mistakes and the guiding question for each

- **Missing `break` → falls into the next case.**  
  Ask: *What happens after the matching `case` when there is no `break` or `return`?*
- **No `default:` → an out-of-range value returns garbage.**  
  Ask: *Which line runs when `cmd` is 42?*
- **Returning the address of a local `char[]` from `command_name`.**  
  Ask: *Where does a string literal live, and where does a local array live?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: which characters belong to which command? Make a table.
2. Concept: several `case` labels can share one body.
3. Point to the case that is missing or falls through.
4. Pseudo-code: switch (c) { case 't': case 'T': return CMD_LED_TOGGLE; ... default: return CMD_UNKNOWN; }
5. Example on a different problem: classifying a char as vowel/consonant/digit.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What does `command_name((command_t)42)` return in your code, and which line produces it?
2. What does `-Wall` tell you if you remove one `case` from a switch over `command_t` without a default?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-04`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Why is `command_t` better than `int` with `#define`s?
2. Show how your code handles a corrupted value.
3. Why is it safe to return a string literal from a function?
