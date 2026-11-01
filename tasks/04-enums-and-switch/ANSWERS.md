# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
1. The compiler can check it. When I switch on a command_t and forget one of its values, the compiler warns me. With #define, the values are just plain numbers, so the compiler has no idea which values belong together and cannot warn.
2. The code explains itself. A function that takes command_t cmd tells the reader exactly which values are expected; an int cmd could be anything. A debugger also shows the name (CMD_LED_ON) instead of just 1.

Another advantage: the enum numbers the values automatically, so two commands cannot accidentally get the same number, and CMD_COUNT always holds the number of commands.
## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
If a switch on an enum has no default and does not handle one of the enum values, -Wall (through -Wswitch) warns: “enumeration value ‘CMD_…’ not handled in switch”. This is valuable when a new command is added later: for example, if someone adds CMD_LED_BLINK to the enum, the compiler points to every switch that forgot to handle it. Without the warning, the new command would silently do nothing (or the function would return a random value), and the bug would only be found when the device misbehaves.
## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
In my code, command_name(42) reaches the default branch and returns "INVALID", and led_next_state(state, 42) also reaches default and leaves the LED state unchanged. An invalid LED state (for example 42 in current) is treated as LED_OFF. This is important because firmware usually runs alone, with nobody watching it. If an impossible value is not handled, the code might return garbage, read outside an array (for example, if names were stored in a table indexed by cmd), or put the hardware into a dangerous state. A defensive default keeps the device in a known, safe state, even when something else has already gone wrong.
