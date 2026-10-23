# Tutor notes: task 06, Your own string functions

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Null-terminated strings
- Array vs. pointer, `sizeof` vs. length
- Bounds: copying at most `size - 1` and always terminating
- Signed vs. unsigned `char`

## Common mistakes and the guiding question for each

- **Off-by-one: writing `dst[dst_size]` for the terminator.**  
  Ask: *What is the index of the LAST byte you are allowed to write?*
- **Forgetting the terminator when the string is truncated.**  
  Ask: *If the source is longer than the buffer, what is in the last byte after your copy?*
- **`dst_size - 1` when `dst_size == 0` → huge number.**  
  Ask: *What is `(size_t)0 - 1`?*
- **Comparing plain `char` so bytes ≥ 0x80 sort wrong.**  
  Ask: *Is `char` signed on this compiler? What is `(char)0xE9`?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: how does a function know where a C string ends?
2. Concept: the buffer size limits how many chars you copy, the terminator always takes one byte.
3. Point to the loop condition or the terminator line.
4. Pseudo-code: n = min(len(src), size-1); copy n bytes; dst[n] = 0; return len(src).
5. Example on a different problem: counting the spaces in a string.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. With `char buf[4]` and source "abcdef", which bytes of `buf` does your function write, and with what?
2. What does your `my_strlcpy` return there, and how does the caller detect truncation?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-06`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Explain how `strcpy(buf, "LED=1")` with `char buf[4]` can crash a different function later.
2. Why compare as `unsigned char`?
3. Walk through your `my_strlcpy` with `dst_size == 1`.
