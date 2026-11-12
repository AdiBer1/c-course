# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
The variable `op` stores the memory address in code space (Flash/ROM) where the machine instructions for `op_add` begin. When `op(2, 3)` executes, the CPU loads the arguments `2` and `3` into registers and performs an indirect branch-and-link instruction (such as `BLX` on ARM) to jump to the address stored inside the `op` register instead of a fixed compile-time target.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
A data-driven lookup table separates the dispatch logic from the list of commands, so adding or removing operations only requires editing table entries without touching control flow or creating deeply nested branches. In firmware, similar function-pointer tables are used for interrupt vector tables, AT/CLI command dispatchers, communication protocol packet handlers, and state-machine transition tables.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
Subtracting two `int32_t` values can cause signed integer overflow when their difference exceeds `INT32_MAX` or `INT32_MIN` (for example, `2000000000 - (-2000000000)`), which is undefined behavior in C and wraps around to return the wrong sign. Instead, my comparator uses explicit relational comparisons (`a < b` returning `-1` and `a > b` returning `1`), avoiding arithmetic overflow completely.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Calling a NULL or uninitialized function pointer causes the CPU to jump to address `0x00000000` or random memory and attempt to decode data as instructions, resulting in undefined behavior or a hard fault. On a microcontroller without an OS or memory protection unit, address `0x00000000` often holds the reset vector or stack pointer, causing silent resets, corrupted peripherals, or a locked-up system that is very hard to debug.