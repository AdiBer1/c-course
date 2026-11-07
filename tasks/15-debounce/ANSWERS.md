# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
A delay loop blocks the CPU: for 20 ms it does nothing else, so other
tasks, interrupts handling and other buttons are delayed or missed. With
several buttons the waits add up. It also wastes power, because the MCU
cannot sleep while it busy-waits. The tick-based debouncer does a tiny
amount of work per tick and returns immediately, so the CPU stays free.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
There are two stable states, RELEASED and PRESSED, plus a counter. In
RELEASED: if raw is 0 the counter is reset to 0 and nothing happens; if
raw is 1 the counter goes up, and when it reaches the threshold the state
changes to PRESSED, a PRESS event is returned and the counter is reset.
PRESSED works the same way in the other direction (raw 0 for `threshold`
ticks gives a RELEASE event). With threshold 5, a glitch of only 3 ticks
raises the counter to 3, then raw returns to the stable value, which
resets the counter to 0, so it never reaches 5 and nothing happens.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
With `static` variables in debounce.c there is only one copy, so the
code could debounce only one button. A struct passed by pointer lets me
have one `debounce_t` per button, each with its own state, counter and
threshold, using the same functions. It also makes the code easier to
test (each test starts with a fresh struct), and the functions do not hide
state, so what they depend on is visible in the arguments.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
With a 1 ms tick and threshold 5, the input has to disagree for 5 ticks
in a row, so the PRESS event comes about 5 ms after the contact settles
(4 to 5 ms, depending on where the settling falls between two ticks). To
choose the threshold for a real button, I would find the longest bounce
time of the button (from the datasheet, or by measuring it with an
oscilloscope or logging the raw input), and set threshold x tick period
a bit longer than it. Too small and bounces get through; too large and the
button feels slow.
