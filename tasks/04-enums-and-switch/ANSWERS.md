# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
First, an enum is its own named type, so function signatures say exactly what they expect (`command_t cmd` instead of a plain `int`), and the compiler knows the full list of values. That is what lets `-Wall` warn when a `switch` forgets one of them. With `#define`s the compiler only sees numbers and cannot warn about anything. Second, enum names are real symbols that the debugger shows by name (`CMD_LED_ON` instead of `1`), and the values are numbered automatically, so adding a new command in the middle cannot create two commands with the same number by mistake. A `#define` is plain text replacement with no type and no scope.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
`-Wall` turns on `-Wswitch`, which reports something like "enumeration value 'CMD_STATUS' not handled in switch". This is valuable because when someone adds a new command to the enum later, every `switch` that does not handle it is reported at compile time. Without the warning the new command would silently fall into the wrong behaviour, and the bug would only show up on the device. Note that the warning only appears when there is no `default`. In my code I list every enum value explicitly and also keep a `default` for corrupted values. With `-Wswitch-enum`, the compiler would warn about missing cases even when a `default` exists.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
Every `switch` in my code has a `default` branch. `command_name(42)` returns "INVALID", and `led_next_state(current, 42)` does nothing and keeps the current LED state. An invalid `current` state is first treated as `LED_OFF`. Firmware runs for a long time without anyone watching it, and RAM can be corrupted by electrical noise, a stack overflow or a bug in another module. If an impossible value is not handled, the function may return garbage, read outside an array, or drive hardware into an unsafe state. Handling it explicitly means the device always ends up in a known, safe state instead of doing something random.