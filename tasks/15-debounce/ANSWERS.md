# Task 15 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. A simpler debounce is: "when the input changes, wait 20 ms with a delay loop, then read it again". Why is that a bad idea in firmware?

Answer:
Blocking delay loops freeze the CPU, preventing it from handling other critical tasks or interrupts.

## Q2. Draw (in words) the states and transitions of your debouncer. What exactly makes it ignore a 3-tick glitch when the threshold is 5?

Answer:
States are Stable Released and Stable Pressed. A glitch resets the counter if it ends before reaching the threshold.

## Q3. Why is all the debouncer's state inside a `debounce_t` struct passed by pointer, instead of `static` variables in debounce.c?

Answer:
Using a struct allows multiple buttons to be debounced independently without sharing state.

## Q4. With a 1 ms tick and threshold 5, what is the delay between the contact settling and the PRESS event? How would you choose the threshold for a real button?

Answer:
The delay is 5 ms. Choose a threshold based on the manufacturer's maximum bounce time.
