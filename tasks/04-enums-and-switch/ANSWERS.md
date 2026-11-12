# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
groups related constants into a dedicated type for better readability and automatic value assignment, whereas #define is just raw text substitution. Additionally, debuggers display the actual enum symbol names instead of meaningless magic numbers.
## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
With -Wall (specifically -Wswitch), the compiler warns if any enum value is missing from the switch. This ensures that when a new command is added later, the compiler immediately flags every switch block that needs to be updated.
## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
The default branch catches the invalid value and keeps the system in a safe, known state. Defensive handling is critical in firmware because hardware glitches or memory corruption can cause unexpected values, and unhandled cases could lead to erratic behavior or system crashes
