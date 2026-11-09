# Task 14: Ring buffer - Answers

1. **How do wrap-around indices work?**
   Indices standardly increment with `(index + 1) % size`, wrapping back to index 0 when reaching capacity.

2. **Why is explicit count tracking used instead of leaving one slot empty?**
   Tracking `count` explicitly allows standard allocation without wasting a buffer slot to distinguish empty vs full states.

3. **What happens during concurrent access from interrupt and main loop?**
   Race conditions may occur on shared variables (`head`, `tail`, `count`); atomic operations or disabling interrupts during update steps prevent partial state reads.