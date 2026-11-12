# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
A blocking 20 ms delay loop halts the CPU, preventing the firmware from servicing other tasks, communication protocols, or sensors during that time. Furthermore, blocking delays cannot be used inside timer interrupt service routines (ISRs) and do not scale when multiple buttons must be monitored simultaneously.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
The state machine has two states: `BTN_RELEASED` and `BTN_PRESSED`. On each tick, if the raw input disagrees with the current stable state, `counter` increments; if it matches the stable state, `counter` resets to 0. A 3-tick glitch increments `counter` up to 3, but on the 4th tick when the input returns to the stable state, `counter` immediately resets to 0 without ever reaching the threshold of 5 or changing state.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
Storing state inside a `debounce_t` instance rather than file-scoped `static` variables encapsulates the data so multiple independent buttons can share the exact same debouncer functions without interfering with each other's counters or states.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
With a 1 ms tick and threshold 5, the delay between the contact settling and the `PRESS` event is 5 ms. For a real button, I would check the switch datasheet or measure its worst-case mechanical bounce duration with an oscilloscope (typically 5–15 ms) and pick a threshold slightly above that maximum bounce time while keeping it short enough to feel instantaneous to a user.