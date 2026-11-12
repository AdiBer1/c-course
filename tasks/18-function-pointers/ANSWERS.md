# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
op stores the starting memory address (Flash/RAM) of the machine instructions for op_add. When op(2, 3) runs, the CPU loads the arguments (2 and 3) into registers (per the ABI), reads the target address from op, and executes an indirect branch/call instruction (such as BLX on ARM) to jump to that address and save the return link.
## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
An operation table separates data from control logic—adding a command only requires adding a table entry without modifying or bloating core branching code, keeping complexity flat and maintainable. In firmware, similar function-pointer tables are used for interrupt vector tables (IVT), CLI command parsers, state machines, and hardware driver abstraction layers (HAL/Vtables).
## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
Subtracting two int32_t values can cause signed integer overflow (which is Undefined Behavior in C and wraps around in two's complement—e.g., INT32_MAX - (-1) becomes negative, inverting the sort order). A safe comparator avoids arithmetic overflow by using direct relational comparisons ((a > b) - (a < b) or explicit if/else returning -1, 0, or 1).
## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Calling a NULL or garbage pointer forces the CPU to fetch and execute instructions from address 0x0 or an invalid/unmapped address, causing Undefined Behavior or a CPU fault (e.g., HardFault/UsageFault). On a microcontroller without an OS or MMU, there is no process isolation to catch the crash cleanly—it can silently execute vector table data as code, corrupt peripheral registers, or lock up the entire device until a watchdog or manual reset occurs.
