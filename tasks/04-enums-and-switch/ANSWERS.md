# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
1. Because when we do switch on enum type the compiler worn if we forget one value unlike in int

2. The `enum` automatically assigns sequential values ​​(starting with `CMD_NONE = 0`, followed by 1, 2, 3...), which prevents the accidental assignment of the same number to two different commands, or the creation of gaps due to a typo.



## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
If there is no default and we forget enum value ,A warning is received.
When a new command is added, the compiler points out every switch statement that needs to be updated. Without this, the code would compile silently, and the command would behave incorrectly only at runtime. Thus, the bug is caught during compilation rather than on the hardware.
## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
What does the code do with 42? `command_name` returns "INVALID," and `led_next_state` returns the current state unchanged. There is no crash, and the output remains valid.

Why does this matter? The firmware lacks an operator or a debugger in the field, and C enums do not prevent out-of-range values. A safe default keeps the device in a predictable state, while "INVALID" allows the anomaly to be detected in the logs.

