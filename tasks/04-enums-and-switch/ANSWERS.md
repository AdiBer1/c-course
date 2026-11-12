# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
2 reasons:
- The compiler is aware of the values contained within an `enum`. Consequently, it can analyze a `switch (cmd)` statement and issue a warning if one of the values is not handled. If `#define` is used, the compiler sees only raw numbers, meaning a case could easily be overlooked.
- An `enum` has its own distinct type name; this allows function signatures to explicitly state what kind of input they expect. Furthermore, debuggers will display the name (e.g., `CMD_RESET`) rather than the underlying integer value (e.g., 5).
Additionally, `enum` values are numbered automatically. This means that adding a new command does not require manual re-numbering, and the `CMD_COUNT` constant remains accurate and up-to-date.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?


Answer:
-Wall turns on -Wswitch. It warns when a switch on an enum does not handle one of the enum values, but only if the switch has no default.
This is useful when a new command is added later: the compiler points to every switch that was not updated, so the new command does not silently do nothing.
In my code every switch has a default, because of the defensive handling in Q3 (a value like 42 must still be handled). The cost is that -Wswitch stays quiet.
I tested it: I added CMD_BLINK to a copy of the enum and compiled with -Wall. There was no warning, and CMD_BLINK just went to default: led_next_state kept the LED state, and command_name returned "INVALID".
When I compiled with -Wswitch-enum, I got a warning in both led_next_state and command_name, even with the default. So -Wswitch-enum gives both things: a safe default and a warning about a forgotten command.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
Simply put, every `switch` statement includes a `default` case. Consequently, the function `command_name(42)` will return "INVALID," and if `led_next_state` is called with the command 42, the current state of the LED remains unchanged. An invalid LED state (such as 7) is handled as `LED_OFF`. Firmware often runs for years without a reboot; therefore, a bit flip or a bug within an Interrupt Service Routine (ISR) are entirely plausible scenarios. Without a `default` case, the function might return meaningless data ("garbage"), which could, in turn, drive the hardware into an unpredictable state. Including a `default` case ensures that the device remains in a known, safe state.
