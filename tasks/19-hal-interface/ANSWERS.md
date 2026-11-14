# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
I need to write a real GPIO implementation for the chip: a write function that sets the pin in the GPIO output register, a read function that reads the input register, and an init that turns on the clock of the port, sets the pins as output or input, and fills a `hal_gpio_t` with these two functions and the port as ctx. Then I pass this `hal_gpio_t` to `led_init` and `button_init`. In `led.c` nothing changes, not even one line. Only the small HAL part is different for each chip.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
`ctx` tells the function which object it works on. The functions are the same code for everyone, and `ctx` is the "data" of this specific instance: which fake, or on a real board which GPIO port. It is like `this` in C++. With a global there could be only one fake in the whole program. If a test makes two fakes, or there are two GPIO ports, they would all write into the same global and mix up. Also the state would be hidden and tests could affect each other. With `ctx` each `hal_gpio_t` points to its own state.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
The teacher's mock is a known good reference, so if my driver passes with it, I know the driver is right. Writing my own fake shows I understand the interface from the other side, as the one who implements it, which is what I would do when I write a real HAL for a board. Testing on a mock is fast, it runs in milliseconds on the PC and in CI, and I don't need a board. I can check the exact sequence of writes (for example that `led_set` does exactly one write with the right level), and I can choose what the button "reads" without pressing anything. On a real board I can only look at the LED, I can't see how many writes happened, and it is hard to repeat the same test every time.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low means the device is "on" when the pin is LOW (0 V). For an LED, it is connected between the supply voltage and the pin, so current flows and the LED lights only when the pin is LOW. For a button, there is a pull-up resistor, so the pin reads HIGH normally, and pressing connects it to ground, so it reads LOW. The driver handles it so the rest of the code only thinks "on/off" and "pressed". If the wiring on the board changes, I change only one flag in init, and not every place in the program that turns on an LED.