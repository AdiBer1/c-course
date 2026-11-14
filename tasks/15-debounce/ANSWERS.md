# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
A blocking delay loop freezes the CPU for 20 ms, preventing the firmware from servicing other buttons, communication interfaces, sensors, or real-time control tasks during that time. Additionally, if the delay is called inside an interrupt service routine (ISR), it blocks lower-priority interrupts and can cause missed hardware events or watchdog resets.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
The debouncer has two stable states: `BTN_RELEASED` and `BTN_PRESSED`. On each tick, if the raw input matches the current stable state, `counter` is reset to 0 and the state remains unchanged; if the raw input differs, `counter` increments, and only when `counter` reaches `threshold` does the state transition to the opposite state and emit `BTN_EVENT_PRESS` or `BTN_EVENT_RELEASE`. When a 3-tick glitch occurs with a threshold of 5, `counter` increments up to 3, and on the 4th tick when the signal returns to the stable state, `counter` immediately resets to 0 without ever reaching 5.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
Storing all state inside a caller-owned `debounce_t` struct makes the module reentrant and allows the firmware to instantiate and debounce multiple independent buttons simultaneously using the exact same functions. Using `static` variables inside `debounce.c` would restrict the module to a single global button instance and cause state corruption if called for more than one input.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
Once the physical contact settles into the pressed state, it takes 5 consecutive 1 ms ticks—a 5 ms delay—for `counter` to reach the threshold and emit the `BTN_EVENT_PRESS` event. For a real button, I would inspect the switch's datasheet for its maximum bounce duration (or measure the worst-case bounce time on an oscilloscope) and set the threshold slightly above that duration (typically 10–20 ms) while keeping it short enough to feel instantaneous to the user.