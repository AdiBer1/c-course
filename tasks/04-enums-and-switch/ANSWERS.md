# Task 04 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `command_t` an `enum` and not just `int` constants like `#define CMD_LED_ON 1`? Name two concrete advantages.

Answer:
1. The values ​​are automatically numbered..
2. Clear debugging.

## Q2. What does `-Wall` warn about if your `switch (cmd)` in `led_next_state` has no `default` and you forget one enum value? Why is that warning valuable when a new command is added later?

Answer:
*/----------------------------------warning!!!!!!!--------------------------------------
src/command.c:55:5: warning: enumeration value 'CMD_COUNT' not handled in switch [-Wswitch]
src/command.c:73:1: warning: control reaches end of non-void function [-Wreturn-type]
/*
The compiler warn that we missed catch cases.
We must put the default, in order that the code will be ok even if we will add values/ cases for our enum.

## Q3. A corrupted byte in RAM (or a bug elsewhere) can leave a `command_t` variable holding 42. What does your code do with it, and why is it important that firmware handles "impossible" values defensively?

Answer:
My code will return the default.
The mechine must stay safety and strong although when something unexpected happened,
becouse on mechine it can be danger!
