# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
Using an `enum` provides strict type safety, allowing the compiler to ensure variables only hold valid command states. Additionally, it improves debugging by displaying symbolic command names instead of raw numeric values.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
The compiler issues a warning when an enum `switch` lacks a `default` and misses a case. This is valuable during maintenance because adding a new command later immediately flags all unhandled switch statements across the codebase.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
Out-of-range values like 42 are safely caught by defensive `default` branches, returning `"INVALID"` or keeping the LED state unchanged. Handling impossible values defensively prevents undefined behavior, system crashes, and safety hazards caused by memory corruption or glitches.