# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
The variable `op` stores the memory address of the entry point instructions for `op_add`. At the CPU level, executing `op(2, 3)` passes arguments through registers/stack per the calling convention and executes an indirect call instruction (`BLX` on ARM, `CALL` on x86) to jump to the address held in `op`.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
An operation table decouples command definition from execution logic, allowing new operations to be added by adding array entries without modifying implementation code or bloating `switch` logic. In firmware, lookup tables are used in interrupt vector tables (IVTs), shell/command dispatchers, and state machine transition tables.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
Returning `a - b` causes integer overflow/underflow when comparing large opposite-sign values (e.g., `INT32_MAX` and `INT32_MIN`), producing an inverted or undefined result. Instead, explicit conditional comparisons (`a > b` return 1, `a < b` return -1) safely compare values without subtraction arithmetic.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Calling a NULL or corrupted function pointer jumps execution to invalid memory. On a microcontroller without Memory Protection Units (MPU), this causes a HardFault exception, silent crash, memory corruption, or unpredictable hardware execution that requires a manual/watchdog hardware reset.