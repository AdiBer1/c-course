# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
op contain the function's address in memory.
when we call op(...):
the cpu store the argument to the register
read the address from op,
Indirect Branch/Call
PC=...
and run from there.


## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
search table seperate the logic and information.
if we will want add more line, we will change just the table.
it more eazy to plan in this way. 

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
Signed integer overflow.
when the diff between a and b so big.
my code return -1, 1 or 0.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
the cpu go to 0 or other address and the program will do unexpected act.

