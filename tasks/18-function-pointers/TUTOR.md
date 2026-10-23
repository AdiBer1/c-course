# Tutor notes: task 18, Function pointers (a gentle intro)

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- A function's name is its address
- Calling through a pointer
- `typedef` for function pointer types
- Tables of `{key, function}`
- Comparator functions

## Common mistakes and the guiding question for each

- **Writing `op_add()` in the table (calls it) instead of `op_add`.**  
  Ask: *Do you want to store the function, or the result of calling it?*
- **Calling a `NULL` function pointer.**  
  Ask: *What does `apply_op` promise when `op` is NULL?*
- **Comparator `return a - b;`.**  
  Ask: *What is `INT32_MIN - 1`?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: what is stored in a variable of type `binary_op_t`?
2. Concept: `op(a, b)` calls whatever function `op` points to.
3. Point to the line that calls a pointer without checking it, or the table entry.
4. Pseudo-code for op_lookup: for each entry: if entry.symbol == symbol: return entry.fn; return NULL.
5. Example on a different problem: an array of `void (*)(void)` LED patterns, played in order.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What is the difference between `op_add` and `op_add(1, 2)`?
2. Why does your comparator not use `a - b`?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-18`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. What happens at CPU level when you call through a function pointer?
2. Why is a table better than a switch for many commands?
3. Where does real firmware use arrays of function pointers?
