# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer: A delay loop blocks the CPU and prevents it from doing other important tasks while waiting. It can make the system less responsive and interfere with real-time operations. A tick-driven debouncer is better because it does a small amount of work on each tick without blocking the CPU.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer: The debouncer has two stable states: BTN_RELEASED and BTN_PRESSED. When the raw input differs from the stable state, a counter increases on every tick. If the input returns to the stable state, the counter resets to zero. The state changes only when the counter reaches the threshold. With a threshold of 5, a 3-tick glitch is ignored because the counter reaches only 3 before resetting.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer: Keeping all the state inside a debounce_t structure allows each button to have its own independent debouncer. The structure stores the stable state, counter, threshold, and press count. Using static variables would share the same state between all buttons, making it difficult to handle multiple buttons independently.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer: With a 1 ms tick and a threshold of 5, the debouncer requires 5 consecutive ticks, so the PRESS event is detected approximately 5 ms after the input starts staying stable in the pressed state. For a real button, I would choose the threshold based on its measured bounce duration and test it under different conditions. The threshold should be long enough to ignore bouncing but short enough to keep the button responsive.

