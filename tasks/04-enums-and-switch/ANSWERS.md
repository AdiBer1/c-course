# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.


## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
An enum makes the code easier to read because it uses meaningful names instead of numbers. It also groups related commands into one type, which makes the code easier to maintain.


## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
The compiler can warn that an enum value is not handled in the switch. This helps catch missing cases, especially when a new command is added later.


## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
My code handles an invalid command with the default case. It keeps the current LED state or returns "INVALID". This helps prevent unexpected behavior if the data is corrupted.
