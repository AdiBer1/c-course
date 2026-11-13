# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
I would write one small, chip-specific HAL implementation: two functions with the same signatures as `write` and `read` in `hal_gpio_t`, which access the real GPIO registers of that microcontroller (for example on an STM32, write sets or clears the pin through the GPIOx->BSRR register and read checks GPIOx->IDR), plus an init that configures the pins and fills a `hal_gpio_t` with those function addresses and a `ctx` (for example a pointer to the GPIO port). The startup code then passes that `hal_gpio_t` to `led_init` and `button_init`. Nothing in `led.c` changes at all: it only calls `gpio->write` and `gpio->read`, so it compiles and runs the same on the PC with a mock and on the board with real hardware. Only the small HAL file is different for each chip.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
`ctx` is the instance data of the implementation: a `void *` that the interface carries around and passes back to every function call, so the function knows WHICH object it is working on. `fake_gpio_init` stores the address of the `fake_gpio_t` in `hal->ctx`, and `fake_write` casts `ctx` back to `fake_gpio_t *` to update that fake's log. With a global, there could only be one fake: two `hal_gpio_t` instances (for example two GPIO ports, or two tests running with separate fakes) would write into the same log and mix up their results. With `ctx`, every interface instance has its own data and the same functions serve all of them. On real hardware `ctx` would point to the specific GPIO port (GPIOA, GPIOB, ...), so one implementation handles all ports.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
The two test from different sides: the teacher's mock checks that my DRIVER uses the interface correctly (it records every call), and my fake shows that I can IMPLEMENT the interface, so the tests prove that both sides of the contract work and that any implementation that follows `hal_gpio_t` can be plugged in. Testing on a mock runs on the PC in milliseconds, without a board, cables or a debugger, and it runs automatically in CI on every push. It sees things you cannot see on a board: the exact sequence of writes, that `led_set` writes exactly once, which level went to which pin. It can also easily simulate situations that are hard to create on real hardware, like a button reading pressed on exactly the third read. On a board you can only look at an LED and guess.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
"Active low" means the device is active when the pin is at the LOW level (0 V). For an LED this is common when the LED and its resistor are connected between the supply voltage and the pin: when the pin drives LOW, current flows from VCC through the LED into the pin and the LED lights up, and when the pin is HIGH both ends are at VCC and no current flows. A button with a pull-up resistor is the same: the pin reads HIGH when released and the button pulls it LOW when pressed. The driver handles it because it is a detail of how the board is wired, not of what the application wants. The application should only think "LED on" or "button pressed". If every caller had to invert the level, the inversion would be spread all over the code, someone would forget it (or do it twice), and changing the board wiring would mean changing the application. With the driver, only the `active_low` flag in `led_init` changes.