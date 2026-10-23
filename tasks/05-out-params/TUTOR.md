# Tutor notes: task 05, Output parameters and error codes

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Pointers: `&x`, `*p`, pass-by-pointer
- `const` on input pointers
- Status codes vs. output values
- Arrays decay to pointers; the length travels separately

## Common mistakes and the guiding question for each

- **Writing `*min_out` before checking `len == 0`.**  
  Ask: *If the function fails, what values do the caller's variables have now?*
- **Swapping the pointers instead of the values.**  
  Ask: *After your swap, which memory has changed: the caller's variables or only your local copies?*
- **Starting min/max at 0 instead of the first element.**  
  Ask: *What does your function return for `{-5, -9}`?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: what does the caller need to give you so you can change its variables?
2. Concept: `*p = value` writes into the caller's variable; `p = ...` only changes your copy.
3. Point to the line that writes an output before all checks are done.
4. Pseudo-code: check NULLs → check len → compute into locals → write outputs → return OK.
5. Example on a different problem: `bool parse_digit(char c, int *out)`.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What happens in your `min_max_i16` if `len` is 0? Which outputs change?
2. In `sort3_i32`, why do you need pointers at all?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-05`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Draw what is on the stack when `swap_i32(&x, &y)` runs.
2. Why must outputs stay untouched on error? Give a firmware example.
3. Why is `values` const and `min_out` not?
