# Tutor notes: task 16, Packet parser

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Byte-driven state machines
- Framing and resynchronisation
- Validating lengths from untrusted input
- Double buffering a result

## Common mistakes and the guiding question for each

- **Storing payload before checking LEN ≤ max.**  
  Ask: *What would LEN = 200 do to your payload array?*
- **Not handling LEN = 0 (waits for a payload byte that never comes).**  
  Ask: *After LEN 0, which state should come next?*
- **Treating 0x7E inside the payload as a new start.**  
  Ask: *How does this protocol know where the payload ends?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to trace one frame byte by byte through the state diagram in the header.
2. Concept: each state knows what the next byte means.
3. Point to the state that mishandles the failing test's byte.
4. Pseudo-code: switch (state) { WAIT_START: if byte == START → LEN; LEN: validate → PAYLOAD or CHECKSUM; ... }
5. Example on a different problem: a parser for `$...*` NMEA sentences.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What does your parser do with `7E 05 ...` if the LEN byte was really lost?
2. Why is there a separate `done[]` array?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-16`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Show the line that protects against a huge LEN, and explain the attack it prevents.
2. How does the parser resynchronise after garbage?
3. XOR vs. CRC here: what does XOR miss?
