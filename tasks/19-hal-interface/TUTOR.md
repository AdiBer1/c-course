# Tutor notes: task 19, A HAL interface and a mock

For the AI tutor (see `TUTOR_RULES.md`). Never give the solution. Students can read this file too, and that is fine.

## Key concepts the student must leave with

- Interfaces as structs of function pointers
- The `ctx` pointer (instance data)
- Mocks and fakes for testing
- Active-low wiring hidden in the driver

## Common mistakes and the guiding question for each

- **Calling `mock_gpio_*` functions from `led.c`.**  
  Ask: *Would your LED driver still work on a board where there is no mock?*
- **Using a global `fake_gpio_t` instead of `ctx`.**  
  Ask: *What happens with two fakes?*
- **Inverting the level twice (or never) for active-low.**  
  Ask: *For an active-low LED that should be ON, which level goes to the pin?*

## Hint ladder (general → specific)

Go one step down only when the previous step did not help.

1. Ask: which functions is the LED driver allowed to call?
2. Concept: `gpio->write(gpio->ctx, pin, level)` is the only way out of the driver.
3. Point to the line that ignores `active_low` or uses a global.
4. Pseudo-code for the fake's write: fake = ctx; if room: log[count++] = {pin, level}; total++.
5. Example on a different problem: a `logger_t` interface with a `print` function pointer, and a fake logger that stores lines.
6. Only after all of the above: a fix of a single line, explained.

## Check questions (ask when she thinks she is done)

1. What would you need to write to run `led.c` on a real board?
2. What is `ctx` for in your fake?

Then remind her: fill in `ANSWERS.md`, run `make score` (60+), open the PR from branch `task-19`.

## Defense questions (asked by the teacher in the oral defense)

These are public on purpose, so she can prepare.

1. Show both sides of the interface: the driver calling it and your fake implementing it.
2. What changes when this code moves to a real microcontroller?
3. Why test against a mock at all?
