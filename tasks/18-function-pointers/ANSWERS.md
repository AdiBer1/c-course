# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
op stores the address of the op_add function. When we call op(2, 3), the program uses that address to jump to the code of op_add and runs it with 2 and 3 as the arguments.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
With a table, each symbol is directly connected to a function pointer, so adding a new command only requires adding another entry to the table. This can be easier to maintain than a large switch. Similar tables are used in firmware for interrupt vector tables, command dispatchers, and state machines.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
The problem is that a - b can overflow when the values are very large or very small. My comparators use comparisons instead of subtraction, so they return -1, 0, or 1 without doing a calculation that can overflow. For cmp_abs, I also use int64_t when calculating the absolute values so that INT32_MIN is handled safely.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Calling a NULL or invalid function pointer causes the program to try to execute code from an invalid address, which can cause a crash or undefined behavior. On a microcontroller this can be especially difficult to debug because it can cause a fault and stop the firmware, and there may not be an operating system to recover from it.
