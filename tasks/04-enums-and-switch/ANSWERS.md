# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
First, enums provide compile-time type safety so the compiler can detect invalid type assignments and ensure function arguments match the expected enum type. Second, using enums allows the compiler to perform exhaustiveness checking in `switch` statements, warning if any enum case is unhandled.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
The compiler issues an `enumeration value not handled in switch` warning. This is valuable when extending code, as adding a new command to `command_t` in the future will automatically highlight all `switch` blocks across the codebase that need to be updated to support it.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
The code handles out-of-range values defensively by treating them as unknown commands or returning fallback safe defaults (such as returning `"INVALID"` for names or keeping the current LED state). Defensive checks prevent undefined behavior, memory corruption, or unsafe hardware actions when cosmic rays or bugs corrupt memory.