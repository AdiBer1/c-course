# Task 14 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. If you only had `head` and `tail` (no `count`), `head == tail` could mean "empty" or "full". How does the struct here solve that, and what is the other common solution?

Answer:
Here - by saving count, if count - counter of the elements in the queue, is 0 - its empty, otherwise - it full, count==size, other common solution - leave a gap of one cell between head and tail, queue is full when head==(tail+1)%size
## Q2. Many implementations require the size to be a power of two and use `index & (size - 1)` instead of a comparison or `%`. Why is that popular on microcontrollers?

Answer:
Because doing % in many micorcontrollers take many clock-cicles, on the other hand - doing AND take usally ~1 clock-cicle, a big differnce.
## Q3. In a UART driver, the receive interrupt pushes and the main loop pops. Which fields does each side write? Why can this be dangerous with the `count` design, and what would you do about it?

Answer:
The UART writes to the head, it is very dengerous because the main task schedualler or so reads from the tail and allso updates count to count--, while the UART makes count++, becasue ++/-- are not atomic, it can couse a RACE CONDITION - this is the classic CONSUMER-PRODUCER problem. it has many ways to solve, the easiest here - to avoid using count, and calculate allways head-tail, plush some kind of indicator if its full/empty as in Q1, or even better - with one cell empty for gap
## Q4. Why does the caller provide the storage array instead of the ring buffer having a fixed internal array?

Answer:
This makes it much more flexable, the size of the array can change easlly without needing to use malloc or so, and because it can be static - no need to use malloc or so for dianamic allocating - we can controller it fully in the memory 