# Tutor notes: task 21, A simulated interrupt

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Interrupt context vs. main loop
- `volatile`
- Race conditions on read-modify-write
- Critical sections
- Single-producer/single-consumer ring buffer

## Common mistakes and the guiding question for each

- **Shared counters not `volatile`.**  
  Ask: *How does the compiler know the ISR can change this variable?*
- **Early `return` inside the critical section, leaving interrupts disabled.**  
  Ask: *On every path out of the function, are interrupts enabled again?*
- **ISR returns without reading the data register when the buffer is full.**  
  Ask: *On real hardware, what clears the RX interrupt?*
- **Doing the line parsing inside the ISR.**  
  Ask: *How long may an ISR take at 115200 baud?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask her to list every variable and mark who writes it: ISR, main loop, or both.
2. Concept: anything written by both sides needs `volatile` and, on the main side, a critical section.
3. Point to the shared update that happens with interrupts enabled.
4. Pseudo-code: disable; if lines > 0: pop bytes until '\n', copy, lines--; enable.
5. Example on a different problem: a timer ISR that increments a tick counter read by the main loop.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. Which of your variables are written by both the ISR and the main loop, and how is each protected?
2. What happens if a byte arrives while `uart_rx_read_line` has interrupts disabled?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-21`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Explain a lost update on `count` step by step.
2. Why `volatile`? What can the optimiser do without it?
3. Why must the ISR read the data register even when the buffer is full?
