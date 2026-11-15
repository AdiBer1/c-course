# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
We'd need to write a small file for the specific board with our own write and read functions. Inside them we'd do the real hardware stuff, like setting the pin's bit in the port register. Then we fill a hal_gpio_t with those functions and pass it to led_init. led.c itself wouldn't change at all, because it doesn't care who's behind write and read. It just calls them.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
ctx tells the function which fake it's working on. The driver doesn't look inside it, it just passes it along. If we used one global fake, there could only ever be one, and two fakes would end up writing into the same log. That's exactly what the test with two fakes checks: writing to one shouldn't show up in the other.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
The teacher's mock checks our driver against code we didn't write, so a bug in our fake can't cover up a bug in the driver. Our fake shows we can also build the other side of the interface, and its log lets us check the exact order of the writes. Testing on a mock is fast and runs on our computers. We don't need to burn the board or stare at the LED to see if it turned on. It can also catch things we'd never see with our eyes, like an extra write or a write to the wrong pin.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low means the LED turns on when the pin is LOW (0 V), not HIGH. It's just how it's wired on the board. Same idea with a button that pulls the pin LOW when you press it. The driver handles it so the rest of the code only has to think "on" or "off" and doesn't need to remember to flip anything. If the wiring changes, we only change one flag in init instead of hunting for every place that turns the LED on.

