# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
An enum makes the code easier to read and understand. It also groups all related commands into one type, which helps prevent mistakes when using different values.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
-Wall can warn that an enum value is missing from the switch statement. This is useful because when we add a new command, the warning reminds us to handle it in the code.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
If the value is 42, the code reache the default case and returns the word invalid or keeps the current led state. It's important because unexpected values can happen, and the firmware should handle them safely instead of behaving incorrectly.
