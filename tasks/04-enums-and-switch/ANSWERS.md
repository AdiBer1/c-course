# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
An `enum` provides type safety, preventing invalid integers from being passed silently to functions expecting a command. Additionally, it improves debuggability by showing descriptive symbol names instead of raw numbers in debuggers, and enables compiler checks in `switch` statements.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
`-Wall` generates a warning stating that an enumeration value is not handled in the switch statement (`enumeration value not handled in switch`). This warning is invaluable because when a new command is added to the enum in the future, the compiler automatically highlights all switch statements across the codebase that need to be updated.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
The code handles unexpected values via the `default` case, returning a safe fallback (such as `CMD_UNKNOWN` or leaving the current state unchanged). Defensive programming is critical in firmware because hardware noise or RAM corruption can alter variables, and unhandled states could cause system crashes or unsafe hardware behavior.

