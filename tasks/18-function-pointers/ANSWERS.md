# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
The variable op stores the memory address of the op_add function. At the CPU level, an indirect branch or jump instruction is executed, setting the instruction pointer (PC) to that stored address so the processor jumps directly to the function's code.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
A lookup table avoids long and repetitive switch-case chains, making the code cleaner, more modular, and easily extensible by simply adding new entries to the table without modifying logic. Such tables are widely used in firmware for command dispatchers, protocol handlers, and peripheral state machine mappings.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
Subtracting two values (a - b) can cause signed integer overflow if the numbers are very large and have opposite signs, resulting in undefined behavior and wrong sorting order. Instead, our comparators safely compare values using relational operators (<, >, ==) or absolute value checks without subtraction overflow.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Calling a NULL or corrupt function pointer attempts to execute code at address zero (or an invalid memory location), triggering an immediate segmentation fault or hard fault. On a microcontroller without an operating system, this usually crashes the device completely, causing a hard system reset.

