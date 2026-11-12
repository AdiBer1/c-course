# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
A blocking delay loop freezes the entire CPU for 20 ms, preventing the microcontroller from servicing other tasks, communication interfaces, or timing-critical logic. Furthermore, if the input was just a brief noise spike rather than an intentional press, the system wastes 20 ms blocked on a false trigger, and if the button is still bouncing after 20 ms, a single delayed read can still sample an unstable state.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
The debouncer tracks the current stable state (`stable`), a candidate state (`candidate`), and a consecutive-sample counter (`count`). When `raw` equals `stable`, `count` resets to 0; when `raw` differs from `stable`, it becomes `candidate` and `count` increments on each consecutive matching tick until `count == threshold`, at which point `stable` updates to `candidate` and emits a PRESS or RELEASE event. During a 3-tick glitch with threshold 5, `count` only reaches 3 before `raw` returns to `stable`, immediately resetting `count` to 0 without ever changing `stable` or firing an event.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
Storing state inside a `debounce_t` struct passed by pointer allows the same debounce module to manage multiple independent buttons or inputs simultaneously without state collisions, makes the code thread/ISR-safe when each instance is owned separately, and enables deterministic unit testing by resetting or inspecting individual instances cleanly.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
Once the mechanical contact settles at its final logic level, it takes 5 consecutive 1 ms ticks for the counter to reach the threshold, resulting in a 5 ms detection delay. For a real physical button, you would measure its worst-case bounce duration on an oscilloscope (typically 5–20 ms depending on switch quality and wear) and set the threshold slightly above that maximum bounce time while keeping total latency low enough (usually under 30–50 ms) so the button still feels instantaneous to the user.