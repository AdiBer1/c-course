# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
`op` holds a pointer to the address where the function `op_add` is stored. When the call is executed, the CPU goes to the function `op_add` (it jumps to the address stored in `op`), calls it, and the function does the calculation. Then the CPU returns to the address in the program where the function was called.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
A table is better than a `switch` when there are many commands, for several reasons. For example, this way we can add more commands to the table without having to change the code where the `switch` is. In addition, with a table we can go over all the commands and do something with them, like printing them or searching for a command, which is not possible with a `switch`. Also, a `const` table can be stored in Flash instead of RAM, and RAM is much smaller than Flash memory.
Another place where tables like this are used is, for example, the interrupt vector table.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
There can be an overflow here outside the range of the type. For example, if `b` is bigger than `a`, but `a - b` is too small to fit in the range of `int`, it will look like a big number and return a positive number.
My function does the comparison with comparison operators, and not with an arithmetic operation.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
On a regular computer, if this happens, the operating system will detect that the address is invalid, or that it is 0 or something like that, and raise an error, and the program may even crash. What happens on microcontroller systems is that the controller doesn't know that the address is garbage: it goes to the place it was told to and runs the data there as if it were the function's instructions, and this is undefined behavior. In addition, there is no error message and no screen, so it will be very hard to know what happened and where.
