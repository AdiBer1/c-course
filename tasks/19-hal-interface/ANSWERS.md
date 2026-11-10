# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
To run led.c on a real board, I just need to write a hardware-specific implementation of the interface—actual write and read functions that access the chip's GPIO registers—and wire them into a hal_gpio_t struct, just like fake_gpio_init does. Nothing in led.c itself needs to change; only that small HAL layer depends on the actual chip.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
ctx passes the specific instance the function should operate on. That way, a single implementation can serve multiple independent instances, whether that's two separate fakes or two different GPIO ports on the same chip. Using a global variable would limit us to a single instance—two fakes would write to the same place and overwrite each other's state, which is exactly what test_two_fakes_are_independent tests.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
The teacher's mock checks the final outcome—the last state of each pin—while my fake records the exact sequence of writes in order, letting me verify things like led_init writing exactly once. Testing on a mock lets the suite run on a PC in milliseconds without any hardware, making it easy to run automatically in CI. It also lets me test edge cases that are hard to reproduce on real hardware and inspect every single read and write, whereas on a physical board you can only see whether the LED turns on.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Electrically, an active-low LED is wired between the positive supply voltage and the GPIO pin. When the pin is driven LOW (0V), current flows and the LED lights up; when it's HIGH, it turns off. Similarly, a button with a pull-up resistor connects the pin to ground when pressed, so pressed equals LOW. If the rest of the codebase had to remember that the logic is inverted, it would be easy to make mistakes, and switching to a board with different wiring would mean updating code everywhere. By handling it inside the driver, the rest of the application only thinks in terms of "on" and "off," and the polarity is configured in one place, when led_init is called.

