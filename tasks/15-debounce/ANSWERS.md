# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
A delay loop blocks the CPU: for 20 ms it does nothing else. In that time the firmware cannot read other buttons, refresh a display, answer a UART command or run a control loop, and if the delay runs inside an interrupt, other interrupts are delayed too and data can be lost. Every press would freeze the whole device for 20 ms. It also does not scale to several buttons, and it reads the input only twice, so a bounce at the exact moment of the second read can still give a wrong result. A tick-driven state machine does a few instructions per tick and returns immediately, so the rest of the firmware keeps running, and it requires the input to be stable for several ticks in a row, which is more reliable.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
There are two stable states, RELEASED and PRESSED, plus a counter. Each tick:
- In RELEASED: raw input "released" -> stay, counter = 0. Raw input "pressed" -> counter + 1, and when the counter reaches the threshold -> go to PRESSED, counter = 0, return PRESS.
- In PRESSED: raw input "pressed" -> stay, counter = 0. Raw input "released" -> counter + 1, and when the counter reaches the threshold -> go to RELEASED, counter = 0, return RELEASE.
A 3-tick glitch is ignored because the counter only grows while the input disagrees with the stable state IN A ROW. After 3 disagreeing ticks the counter is 3, which is less than 5, so nothing happens, and as soon as the input agrees again the counter is reset to 0. The glitch never reaches the threshold, and the next glitch starts counting from 0 again.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
A device usually has several buttons, and each one needs its own stable state, counter and press count. With a struct, each button gets its own `debounce_t` instance, and the same functions work for all of them: `debounce_update(&btn_up, ...)` and `debounce_update(&btn_down, ...)` do not affect each other. With `static` variables there would be only one copy, so all buttons would share the same counter and state and mix each other's input. A struct also makes the code easier to test (each test creates a fresh instance) and makes it clear exactly which data a function uses. And there is still no malloc: the caller can declare the structs `static`.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
Once the contact has settled, the input disagrees with the stable state on every tick, and the PRESS event comes on the 5th consecutive tick, so the delay is about 5 ms (between 4 and 5 ms, depending on where in the tick period the contact settled). To choose the threshold, I would look at the button's datasheet or measure the bounce with an oscilloscope. Typical mechanical buttons bounce for about 1-10 ms, some cheap ones up to 20 ms. The threshold times the tick period should be longer than the worst bounce (with some margin), but still short enough that the user does not notice a delay: anything under about 30-50 ms feels instant. With a 1 ms tick, a threshold of 10-20 is a common choice.