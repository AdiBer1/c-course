# Tutor notes: task 09, A static memory pool

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Static storage duration
- Alignment and `max_align_t`
- Bookkeeping: used flags or a free list
- Pointer arithmetic and comparing addresses safely via `uintptr_t`
- Fragmentation

## Common mistakes and the guiding question for each

- **`static uint8_t storage[8 * 32];` without alignment.**  
  Ask: *At which addresses can a `uint64_t` or `double` safely be stored? Is your storage guaranteed to start there?*
- **Accepting `p + 1` as a valid block.**  
  Ask: *How do you know an address is exactly the start of a block, not just inside the pool?*
- **Comparing unrelated pointers with `<`.**  
  Ask: *What does the C standard say about `<` between pointers into different objects?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: what does the pool need to remember about each block?
2. Concept: an array of blocks plus one flag per block is enough.
3. Point to the check in `pool_free` that lets a bad pointer through.
4. Pseudo-code: offset = addr - start; valid if 0 <= offset < total and offset % BLOCK == 0 and in_use[offset / BLOCK].
5. Example on a different problem: a fixed table of 8 timer slots with an `active` flag each.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What exactly happens in your code on `pool_free(p); pool_free(p);`?
2. How do you guarantee alignment of every block?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-09`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Where does the pool's memory live, and when is it reserved?
2. Show how a pointer into the middle of a block is rejected.
3. Why can a pool not fragment?
