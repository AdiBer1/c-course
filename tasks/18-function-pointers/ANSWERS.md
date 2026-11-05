# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
The variable `op` stores the memory address of the first instruction of `op_add` in executable memory (Flash/ROM). At the CPU level, executing `op(2, 3)` loads the arguments into registers according to the calling convention and performs an indirect call/branch instruction (e.g., `BLX` on ARM) to the target address stored in `op`.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
An operation table decouples command lookup from control logic, allowing commands to be added, modified, or registered dynamically without modifying giant `switch` statements or conditional branching code. In embedded firmware, similar function-pointer tables are used in Interrupt Vector Tables (IVTs), UART/CLI command dispatchers, state machine transition tables, and hardware abstraction layer (HAL) driver interfaces.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
Returning `a - b` is dangerous because integer subtraction can overflow or underflow when comparing extreme values (for example, comparing `INT32_MIN` and a positive number produces a wrapped positive result, yielding the wrong comparison order). My implementation uses explicit conditional checks (`a < b` and `a > b`) to safely return -1, 1, or 0 without arithmetic operations that could overflow.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Dereferencing and calling a `NULL` or garbage function pointer causes an indirect jump to address `0x0` or an unaligned/invalid memory location. On microcontrollers without an MPU/MMU, this does not raise a clean operating system crash; instead, it triggers a HardFault exception, jumps into the Interrupt Vector Table, or causes unpredictable code execution and hardware lockups, making it difficult to debug.

