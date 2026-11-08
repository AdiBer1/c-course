# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
op holds the address where the code of op_add starts. When op(2, 3) runs, the CPU puts 2 and 3 in registers, saves where to come back to, and jumps to the address stored in op. op_add runs, puts the result in a register and jumps back.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
To add a command I just add one line to the table, and the search code never changes. With a switch I’d have to edit the code every time, and it gets long and messy. You find such tables in the interrupt vector table, in command dispatchers, in state machines and in driver interfaces.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
a - b can overflow: with a = 2,000,000,000 and b = -2,000,000,000 the result doesn’t fit in int32_t, it’s undefined behaviour, and in practice it comes out negative, so the order is wrong. My version only compares: (a > b) - (a < b) always gives -1, 0 or 1, and never overflows.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
The CPU jumps to address 0 (or to a random address) and tries to run whatever is there. On a PC the operating system stops the program right away with a crash. On a microcontroller there is often no such protection: address 0 is real memory, so the CPU either gets a HardFault or runs random code, which can do strange things without any error message.
