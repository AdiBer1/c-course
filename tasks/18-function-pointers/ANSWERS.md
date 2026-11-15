# Task 18 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. In `binary_op_t op = op_add;` and `op(2, 3)`, what is stored in `op`, and what happens at the CPU level when the call runs?

Answer:
`op` just holds an address, the place in memory where the code of `op_add` starts. Writing `op_add` without parentheses doesn't run anything, it only gives you that address. When I write `op(2, 3)`, the CPU puts 2 and 3 where the function expects its arguments (usually in registers), remembers where to come back to, and jumps to the address stored in `op`. The function runs, puts the result in a register and jumps back. So it's basically a normal function call, except the CPU first has to read where to jump.

## Q2. Why is the operation table (`OP_TABLE`) better than a `switch` on the symbol when the list of commands grows? Where else do you find such tables in firmware?

Answer:
With the table, adding a new command is just adding one more line like `{'/', op_div}`. The search loop stays exactly the same. With a `switch` I'd have to go into the code and add another `case` every time, and with lots of commands it gets long and messy. The table also keeps everything in one place, so it's easy to see which symbol goes to which function. In firmware you see this all the time: the interrupt vector table, command tables for things that come in over UART, state machines where each state has its own function, and drivers that are a struct of functions like init/read/write.

## Q3. `int cmp(int32_t a, int32_t b) { return a - b; }` is a common comparator. Why is it wrong, and what does your version do instead?

Answer:
It looks fine but it breaks with big numbers. For example `INT32_MIN - 1` is too small to fit in an `int32_t`, so it overflows. In C that's undefined behaviour, and in practice the number usually wraps around to a big positive one. Then the comparator says INT32_MIN is bigger than 1, and the sort comes out wrong. In my version I don't subtract at all, I only compare: `(a > b) - (a < b)`. Each comparison is 0 or 1, so the answer is always -1, 0 or 1 and nothing can overflow. In `cmp_abs` I had a similar problem, because the absolute value of INT32_MIN doesn't fit in `int32_t`, so I calculate it as `uint32_t`, where it does fit.

## Q4. What happens if a function pointer is NULL (or holds garbage) and you call it? Why is this especially nasty on a microcontroller?

Answer:
The CPU just jumps to whatever address is in the pointer, without checking anything. On a computer with an operating system, address 0 is protected, so the program crashes right away with a segfault. That's annoying, but at least you know something is wrong. On a microcontroller there's usually no OS and nothing protecting address 0, and sometimes it's even real memory (on Cortex-M that's where the vector table is). So the CPU might try to run data as if it were code. If you're lucky you get a HardFault. If you're not, it keeps running on garbage and does weird things, and the bug shows up somewhere totally different from where it started. That's why I check every function pointer for NULL before I call it.

