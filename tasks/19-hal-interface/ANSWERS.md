# Task 19 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Your `led.c` never includes a hardware header or touches a register. What would you need to write to run it on a real board, and what in `led.c` would change?

Answer:
I would write a new implementation of the hal_gpio_t interface for the
board: a `write` function that sets the real port register for the pin,
a `read` function that reads the port's input register, and a `ctx`
that points to the port (for example its base address). Then I would
fill a hal_gpio_t with these functions and pass it to led_init(). Nothing
in led.c changes, because it only calls gpio->write and gpio->read.

## Q2. What is `ctx` for? Why can't `fake_write` just use a global `fake_gpio_t`?

Answer:
`ctx` is a pointer that is stored in the hal_gpio_t and passed back to
write() and read() on every call, so the function knows which instance it
works on. If fake_write used one global fake_gpio_t, there could be only
one fake, and two LEDs on two independent mocks (or two GPIO ports on a
real board) would write into the same data. With ctx, each hal_gpio_t
has its own fake_gpio_t. It also makes tests independent of each other,
because each test starts with its own fresh fake.

## Q3. Why are the teacher's mock and your fake both useful? What does testing the LED driver on a mock give you that testing on the real board doesn't?

Answer:
A mock lets me test the driver without hardware, anywhere and fast, and
in CI on every push. I can check exactly what the driver did: how many
writes, in which order, with which levels, and that it did not read
when it should not. I can also simulate things that are hard to create
on a real board, like a pressed button at a chosen moment, or a pin
number out of range. On a real board I can only see the LED light up;
I cannot see "exactly one write" or the order of writes. Writing the fake
myself also shows me the other side of the interface.

## Q4. What does "active low" mean electrically, and why does the driver handle it instead of the code that calls `led_set`?

Answer:
Active low means the LED is wired between the supply voltage and the
pin, so it lights up when the pin is driven LOW (0 V, the pin sinks the
current) and goes off when the pin is HIGH. A button can be wired the same
way, pulling the pin LOW when pressed. The driver handles it so that the
rest of the firmware only thinks "on/off" and "pressed". If every caller
of led_set had to remember the wiring, one mistake would invert an LED,
and a board revision with a different wiring would require changes
everywhere. With the driver, the wiring is in one place, and a change
is only the `active_low` flag in led_init.
