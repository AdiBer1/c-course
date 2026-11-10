# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
Our struct solves the problem by also keeping a `count` variable: we check whether it is 0 (empty) or equal to `size` (full).
Another common solution is to always leave one slot in the array empty. It wastes a little memory, but this way `head == tail` happens in only one case: when the buffer is empty.

## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
The expression `size - 1` is actually a mask: a mask of ones whose value is the highest index. When we do `&` between the index and the mask, we keep only the index values that are inside the range, and if the index goes past the range, the high bits are cut off and it becomes a small number again, because we got back to the start of the buffer. An effective mask like this is only possible with powers of 2. This is popular on microcontrollers because many of these systems often have no division instruction, so `%` is slow, while `&` is a single fast operation.


## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
`push` touches the `head` variable and `pop` touches `tail`. The problem is that a race condition can happen, and then there can be bugs with the variable that both of them share, `count`. The `--` and `++` operations are not atomic, so things can go wrong there: one update can get lost, and then `count` won't match the real number of bytes.
What I could do to avoid this is to lock the critical section of the code, so that each time only one side can access the `count` variable (on a microcontroller, for example, by briefly disabling interrupts).
(Finally a use for what we learned in the Operating Systems course :)

## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
Because sometimes we may want a buffer of a different size than the default.

