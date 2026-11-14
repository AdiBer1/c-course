# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer: 
To run it on a real board, you would only need to write a concrete HAL implementation (such as configuring the microcontroller's specific GPIO port registers and providing `write` and `read` functions) and initialize a `hal_gpio_t` structure pointing to them. Nothing whatsoever in `led.c` would change, because it depends entirely on the abstract HAL interface rather than chip-specific details.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer: 
The `ctx` (context) pointer passes the specific instance data (like pin states and write logs) to the HAL function, allowing the driver to operate on multiple independent hardware or mock instances. Using a global variable would restrict you to only one fake GPIO instance, break reusability, and cause race conditions or shared state issues if multiple drivers or tests ran concurrently.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer: 
Testing on a mock gives you instantaneous execution, automated assertions, deterministic software control, and the ability to run unit tests on a host PC without needing any physical hardware. While the teacher's mock provides pre-built verifications for specific test suites, your own fake gives you a programmable internal state and log history to validate exact sequences.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer: 
"Active low" means that pulling the electrical pin to a low voltage level (0V/GND) turns the component on, while a high voltage turns it off. The driver handles this so that the rest of the application logic can work with intuitive logical states (`true` for on, `false` for off) without needing to worry about the specific electrical wiring of the hardware.