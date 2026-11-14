# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
The variable op stores the memory address (the PC-relative entry point) of the op_add function in code memory. At the CPU level, an indirect branch or call instruction (such as BLX or CALL with a register) loads that target address into the Program Counter, causing the processor to jump directly to the function's instructions and begin execution.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
An operation table allows O(1) or O(N) data-driven dispatch without branching overhead, avoiding massive nested conditional chains or jump tables that become unmaintainable as commands grow. Similar tables are found everywhere in firmware, such as interrupt vector tables, peripheral driver operation structs, and state machine transition tables.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
It is wrong because subtracting two large signed integers can cause integer overflow or underflow (e.g., subtracting a very negative number from a positive one), wrapping around and inverting the sign of the result. Our version uses relational expressions (a > b) - (a < b), which safely returns -1, 0, or 1 without any arithmetic overflow risk.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Calling a NULL or garbage function pointer causes an immediate segmentation fault or attempts to execute invalid machine instructions as code. On a bare-metal microcontroller without an operating system memory protection unit, this triggers a hard fault trap, crashing the system instantly or causing unpredictable erratic behavior with no stack trace or recovery mechanism.

