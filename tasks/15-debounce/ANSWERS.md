# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
While the delay loop runs, the CPU does nothing else for 20 ms. Other buttons, the UART, the display, everything is stuck. You can't put a delay like that inside an interrupt at all. With 4 buttons it gets even worse. Also, reading only one time after 20 ms is not safe, the read can still happen during a bounce. With the tick method each call takes almost no time and returns right away, so the rest of the program keeps running.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
There are two stable states, RELEASED and PRESSED, and a counter.
- In RELEASED: if the input reads "pressed", counter++. If it reads "released", counter = 0. When counter reaches 5, go to PRESSED, counter = 0, return PRESS.
- In PRESSED: the same but the other way. When counter reaches 5, go to RELEASED, counter = 0, return RELEASE.
In a 3-tick glitch the counter goes 1, 2, 3, and then the input goes back to the stable value, so the counter is reset to 0. It never reaches 5, so the state does not change and no event is returned. The important part is "in a row": one tick of agreement starts the count from zero again.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
So we can have many buttons. Each button gets its own `debounce_t` with its own state and counter, and the same code works for all of them. With `static` variables there is only one copy, so all buttons would share the same counter and mix each other up. It is also easier to test, because every test can start with a fresh struct.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
About 5 ms. After the contact stops bouncing the input needs to be "pressed" for 5 ticks in a row, and the PRESS comes on the 5th tick (it can be a bit more, depending on when the tick comes compared to the moment it settles). For a real button I would look at the signal with an oscilloscope or check the datasheet to see how long it bounces, usually a few ms up to around 10 ms. Then I would pick a threshold a bit longer than that, for example 10 to 20 ms. Not too big, because then the button feels slow to the user.