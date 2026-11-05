# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
Blocking delay loops freeze the CPU, preventing it from handling other critical tasks during the wait time. In embedded systems, blocking is unacceptable because the processor must remain responsive to other sensors, interrupts, and communication protocols.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
The debouncer compares the raw input against the stable state. If they disagree, a counter increments on each tick; if they agree again, the counter resets. A 3-tick glitch only increments the counter to 3, which falls short of the threshold of 5, causing the glitch to be ignored and the counter to reset.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
Using static variables inside the source file would limit the program to a single global button. Passing a debounce_t pointer allows the code to handle multiple independent buttons simultaneously, since each button instance maintains its own isolated state and counter.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
The delay is 5 milliseconds (5 ticks $\times$ 1 ms per tick). For a real button, you choose a threshold by measuring how long the physical contact bounces (typically 5 to 20 ms) and setting the threshold slightly higher to ensure all bouncing has settled.

