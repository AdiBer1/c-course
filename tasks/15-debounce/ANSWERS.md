# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer: 
Using blocking delay loops freezes the entire CPU, preventing it from servicing critical real-time tasks, handling interrupts, or responding to other system events. In embedded firmware, code must never block or wait idly, as responsiveness and predictable timing are essential.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer: 
The debouncer holds a stable state (RELEASED or PRESSED) and tracks consecutive disagreement ticks using a counter. When raw input differs from the stable state, the counter increments; if it equals the stable state, the counter resets to zero. A 3-tick glitch causes the counter to reach 3 and then immediately reset when the signal bounces back, failing to reach the threshold of 5, thus leaving the stable state unchanged.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer: 
Using a struct passed by pointer allows the module to be reentrant and handle multiple buttons independently. If static variables were used inside `debounce.c`, the state would be shared globally, making it impossible to debounce more than one physical button simultaneously.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer: 
With a 1 ms tick and a threshold of 5, the delay is exactly 5 ticks (5 milliseconds) after the contact has fully settled. For a real button, you choose the threshold based on the manufacturer's bounce time specification and your tick period (typically aiming for a duration slightly longer than the maximum expected bounce period, such as 10–20 ms total).