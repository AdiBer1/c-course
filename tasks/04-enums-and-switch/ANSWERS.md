# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:First, using an `enum` improves type safety and readability by grouping related values under a distinct type, allowing the compiler and static analysis tools to verify context. Second, when using an `enum` inside a `switch` statement without a `default` case, the compiler can warn if any enumeration constant was accidentally omitted.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:The compiler emits a `-Wswitch` warning indicating that an enumeration value is not handled in the switch. This is valuable when extending the firmware later, as adding a new command to the enum immediately highlights every switch statement across the codebase that needs to be updated to handle the new state.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:The code handles unexpected values defensively via a fallback `default:` branch or validation check, returning a safe default state (such as leaving the LED state unchanged or returning an unknown command representation). In embedded firmware, memory corruption, electrical noise, or pointers running out of bounds can create invalid states, and defensive handling prevents unpredictable behavior, system lockups, or hazardous physical actions.

