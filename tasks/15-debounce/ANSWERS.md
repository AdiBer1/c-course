# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer: A delay loop blocks the CPU, so nothing else runs while it waits (other buttons, communication, timers). It is also unreliable, because one read after 20 ms can still land on a bounce.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer: There are two states, RELEASED and PRESSED. In each one, every tick where the input differs from the state adds 1 to the counter, and a tick where it matches resets the counter to 0. When the counter reaches the threshold, the state flips and PRESS or RELEASE is returned. A 3-tick glitch with threshold 5 only brings the counter to 3, then the input matches the state again and the counter resets to 0, so nothing happens.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer: With a struct each button has its own state, so I can debounce many buttons with the same code. Static variables would be shared by all buttons and they would disturb each other. The struct is also easy to test.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer: About 5 ms, because the counter needs 5 ticks in a row. For a real button I would measure the bounce time (datasheet or oscilloscope, usually a few ms up to about 20 ms) and choose a threshold a bit longer than that, but not so long that the button feels slow.