# Task 15: Debounce - Answers

1. **Why is mechanical switch debouncing necessary in embedded systems?**
   Mechanical contact bounce causes rapid false voltage transitions that microcontrollers register as multiple presses unless filtered.

2. **How does the counter-based debouncing method work?**
   It samples the raw input per tick; if it differs from the stable state, a counter increments until reaching threshold, which flips the state and generates an event.

3. **What is the trade-off between threshold ticks and response delay?**
   A higher threshold improves noise immunity but adds latency to detecting button events.