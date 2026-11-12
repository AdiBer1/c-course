# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
A function's name without parentheses is its address: the place in program memory (flash on an MCU) where its machine code starts. So `op` is just a variable that holds that address, for example 0x08000124. When `op(2, 3)` runs, the CPU puts the arguments 2 and 3 where the calling convention expects them (on ARM in registers r0 and r1), loads the address from `op` into a register, and does an indirect branch-with-link to it (on ARM `BLX r3`). That jumps to the start of `op_add` and saves the return address. `op_add` runs, puts its result in the return register, and returns to the caller. The only difference from a normal call is that the target address comes from a variable at run time instead of being fixed in the instruction.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
With a table, the data (which symbol maps to which function) is separate from the logic (the lookup loop). Adding a command means adding one line to the table, and the lookup code never changes, so it cannot get a new bug. With a `switch`, every new command means editing the switch, and with dozens of commands it becomes a long, error-prone block. The table can also be `const`, so it lives in flash, can be printed (for example as a "help" list of all commands), and can even be extended by other modules. In firmware such tables are everywhere: the interrupt vector table is an array of function pointers to the interrupt handlers, command-line interpreters and AT-command parsers map command names to handlers, state machines can use one handler function per state, and RTOS tasks and timers are started from function pointers.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
`a - b` can overflow. For example, with `a = INT32_MIN` and `b = 1`, the true difference is -2,147,483,649, which does not fit in an `int`. That is signed overflow, undefined behaviour, and in practice it wraps to a large positive number, so the comparator says INT32_MIN is GREATER than 1 and the sort puts it in the wrong place. The same happens with `INT32_MAX` and a negative number. My version returns `(a > b) - (a < b)`. Each comparison is 0 or 1, so the result is always -1, 0 or 1, and no arithmetic on the values themselves can overflow. In `cmp_abs` I also compute the absolute values in `int64_t`, because `-INT32_MIN` does not fit in `int32_t`.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
Calling it makes the CPU jump to address 0 (or to the garbage address) and try to execute whatever is there as code. That is undefined behaviour. On a PC, address 0 is not mapped, so the operating system stops the program immediately with a segmentation fault, and the bug is found quickly. On a microcontroller there is no memory protection: address 0 is often the start of flash or the vector table, so the CPU happily executes the reset vector or random data as instructions. It may jump into the middle of other code, write to random registers, or end in a HardFault much later and far from the cause. Or it may partly "work" and do something dangerous with the hardware. That is why `apply_op` checks for NULL and returns the fallback instead of calling it.