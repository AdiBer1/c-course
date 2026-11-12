# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
`op` holds an address: the address in memory (in flash on a microcontroller) where the code of `op_add` starts. It is just a number, like a normal pointer to a variable. When `op(2, 3)` runs, the CPU puts 2 and 3 in the registers for arguments, like in any call. Then it loads the address from `op` into a register and jumps to that address (an "indirect" jump, for example BLX on ARM), and saves the return address so it can come back. The only difference from a normal call is that the address is read from a variable at run time and not written in the code.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
With a table, adding a new command is adding one line `{symbol, function}`. The lookup code doesn't change at all. With a switch, the function gets longer and longer and the logic and the list of commands are mixed together. A table is also data, so it can be `const` and stay in flash, and you can loop over it, for example to print a list of all commands. In firmware you find this in the interrupt vector table (an array of handler addresses), in command handlers by message ID in a protocol, in a CLI over UART, and in state machines with one function for each state.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
`a - b` can overflow. For example a = 2,000,000,000 and b = -2,000,000,000: the real answer is 4,000,000,000, which doesn't fit in int32_t. Signed overflow is undefined behaviour, and in practice the result becomes negative, so the sort thinks a is smaller than b and puts them in the wrong order. My version doesn't do any math with the values, it only compares them: `(a > b) - (a < b)`. Each comparison gives 0 or 1, so the result is always -1, 0 or 1 and can never overflow.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
The CPU jumps to that address and tries to run whatever is there as code. On a PC with an operating system, address 0 is protected, so the program crashes right away with a segmentation fault and you see the bug. On a microcontroller there is often no protection, and address 0 can be real memory (for example the start of the vector table). So the CPU runs data as if it was code, and maybe gets a HardFault, or maybe just does strange things and keeps going. With a garbage pointer it can jump anywhere. The bug can show up much later and far from the real cause, and it is very hard to find. That's why `apply_op` checks for NULL before the call.