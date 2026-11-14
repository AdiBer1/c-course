# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
The variable `op` stores the memory address in code space (Flash/ROM) where the machine instructions for `op_add` begin. When `op(2, 3)` executes, the CPU places the arguments `2` and `3` into the argument registers, loads the target address from `op` into a register, and performs an indirect branch-and-link instruction (such as `BLX` on ARM) that saves the return address and jumps the Program Counter (PC) to that function address.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
A lookup table separates the command data from the dispatch logic, meaning adding or removing commands only requires editing a single table entry rather than modifying control-flow branches, and it can be stored cleanly in read-only Flash memory (`const`). Similar function-pointer tables are used in firmware for hardware interrupt vector tables, serial/CLI command parsers, finite state machines, and hardware abstraction layer (HAL) driver interfaces.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
Subtracting two `int32_t` values can cause signed integer overflow, which is undefined behavior in C and yields the wrong sign when comparing values with large differences (such as `INT32_MAX` and `-1`, or `INT32_MIN` and `1`). Instead of subtracting, my comparators use direct relational comparisons (`<` and `>`) to return `-1`, `0`, or `1`, and `cmp_abs` widens to `int64_t` before negating so `INT32_MIN` does not overflow.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Calling a `NULL` or corrupted function pointer causes the CPU to jump to address `0x00000000` or an invalid/non-executable memory region, resulting in undefined behavior or a hardware fault. On a microcontroller, address `0x00000000` often holds the initial stack pointer or reset vector rather than protected unmapped memory, so jumping to `NULL` can execute vector data as instructions or silently reset/lock up the device without any operating system error message.