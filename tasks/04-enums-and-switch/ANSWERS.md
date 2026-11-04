# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
Two advantages of `enum`:
1. With an `enum`, if we accidentally return a value of one enum while the function is expected to return a value of a different enum, the number is the same (for example `1`), but the compiler can warn about the logic bug, because each enum is its own type.
2. If we forget to write a `case` for one of the enum's values, the compiler can warn about it. With plain `int` values it cannot know what the values are supposed to be.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
The warning is: `enumeration value 'CMD_UNKNOWN' not handled in switch`(for exemple).
This is very important, because if we add more values to the enum later, the compiler makes sure we didn't forget to add them to the `switch`.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:

My code checks whether the value of `cmd` is valid. If it isn't, no action is taken (the `default` case keeps the LED state unchanged). This is very important, because systems can't always cope with wrong or unexpected values.