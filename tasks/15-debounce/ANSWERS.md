# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
A delay loop blocks the CPU. For 20 ms nothing else runs: no communication, no display updates, no other buttons. If it runs inside an interrupt, it is even worse. Also, one read after 20 ms is a guess: if the contact is still bouncing at that moment, we may read the wrong value and miss a real press. The tick-based debouncer never waits and checks the input over several ticks.
## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
There are two stable states: RELEASED and PRESSED. The raw input is compared to the stable state on every tick. If it differs, a counter goes up. If it matches, the counter resets to 0. When the counter reaches the threshold, the stable state flips and an event (PRESS or RELEASE) is returned. A 3-tick glitch with threshold 5 is ignored because the counter only reaches 3, then the input goes back to matching the stable state, the counter resets, and nothing happens. Example: 1 1 1 0 ... gives counter 1, 2, 3, then 0.
## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
If the state were in static variables, all buttons would share one counter and one state, and they would interfere with each other.
## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
The delay is about 5 ms (5 ticks × 1 ms). To choose the threshold, find the button’s bounce time (datasheet or oscilloscope) and pick a threshold so that threshold × tick is slightly longer than it. Too low lets bounces through; too high makes the button feel slow.

