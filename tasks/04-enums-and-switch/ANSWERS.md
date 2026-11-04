# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
Using `enum` is better than `#define` for two main reasons:
1. **Type Safety:** The compiler checks that functions receive a `command_t` type and not just any random `int`, which helps prevent passing wrong values by mistake.
2. **Easier Debugging:** When using a debugger, it shows the actual enum name (like `CMD_LED_ON`) instead of just a raw number like `1`.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
If a `switch` statement on an enum is missing a case and has no `default` branch, `-Wall` triggers a warning like `enumeration value not handled in switch`. 
This is really helpful because if we add a new command to the enum in the future, the compiler will instantly warn us about every `switch` in our project that needs to be updated to handle the new value.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
If an invalid value like `42` is passed, our code catches it in the `default` branch of the `switch` statement and keeps the current LED state unchanged (and `command_name` returns `"INVALID"`).
This defensive check is critical in firmware because memory corruption or electrical noise can flip bits in RAM. Handling unexpected values safely prevents the hardware from entering an unknown state or crashing.
