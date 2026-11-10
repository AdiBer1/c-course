# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
Suppose start = 4294966000, which is right before the uint32_t maximum of 4294967295. With now >= start + 5000, the addition overflows and wraps around to 3704. Just half a second later, now = 4294966500, and the check 4294966500 >= 3704 evaluates to true—so the red light ends after 500 ms instead of 5000 ms.
Using now - start >= 5000 avoids this: at 500 ms, now - start is 500, which is false. Even after now wraps around (for example, now = 704), unsigned subtraction wraps in the exact same way and still gives the true elapsed time of 2000 ms.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
With a blocking wait loop, the entire system would freeze for the full 5 seconds—no button reads, no communication, and no other tasks could run, since on a single-core MCU without an OS only one function executes at a time. In the unit tests, the fake clock only advances when the test explicitly advances it, so a blocking loop would hang forever until the test runner kills it after 5 seconds ("infinite loop or too slow"). On top of that, test_update_without_time_passing_returns_immediately specifically verifies that calling update when no time has passed returns right away.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Resetting the timer to now pushes any slight delay forward, so each cycle gets stretched by that delay and the schedule drifts over time. The upside is that every state is guaranteed its full duration, which matters for safety in a traffic light so the yellow phase never gets cut short.
Using start + duration keeps the schedule locked to the clock with zero drift, which is better when you need a steady, exact rate—like synchronized traffic lights down a street, sampling a sensor every 10 ms, or keeping a clock. The downside is that after a big delay, the next state gets shortened, or you can get several rapid transitions in a row trying to catch up.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
Moving to a real microcontroller requires:
- A real hal_gpio_t implementation with write and read functions that    access the GPIO registers, plus mapping pins 0–3 to the actual physical pins on the board.

- A real hal_clock_t implementation where now_ms returns a millisecond counter incremented in a timer interrupt (like SysTick).

- A main loop that repeatedly calls tl_update.

Nothing in traffic_light.c itself needs to change.

