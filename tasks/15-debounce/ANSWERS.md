# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
Using a delay loop is a bad idea because it blocks the program for 20 ms. During this time, the system cannot perform other tasks. A tick-based state machine is better because it checks the button without blocking the program.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
The state machine has two states: BTN_RELEASED and BTN_PRESSED. If the raw input is different from the stable state, the counter increases. When the counter reaches 5, the state changes. If the input returns to the stable state before 5 ticks, the counter resets to zero. This is why a 3-tick glitch is ignored.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
We store the state inside the debounce_t struct so each button has its own state and counter. Using static variables would make all buttons share the same data. This allows us to manage multiple buttons independently.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
With a 1 ms tick and a threshold of 5, the PRESS event is generated after 5 ms of stable input. For a real button, I would choose the threshold based on the button's bounce time and test it in practice.

