# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
-It provides strict type safety, allowing the compiler to ensure that variables only receive valid values belonging to that specific command set. 
-It provides a clear logical grouping of all commands under one umbrella and improves debugging since names are clear instead of numbers.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
The -Wall flag warns when a switch statement on an enum type does not cover all possible values and lacks a default case. This warning is good because if a new command is added to the enum, the compiler highlights where it was forgotten in the code, preventing silent bugs in the system.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
Such a value is handled by the default case in the code, treating it as an unknown command or keeping the current LED state unchanged. Defensive handling like this is crucial in embedded systems to prevent crashes, unexpected behavior, or hazardous hardware actions in the event of random memory corruptions.