# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
Say RED starts at start = 4294966296 (1000 ms before the wrap).
start + 5000 wraps to 4000. So at now = 4294966297 (1 ms later),
now >= 4000 is true and RED ends right away.
With subtraction: at now = 4000 (after the wrap), now - start also wraps
and gives exactly 5000, the real time that passed. So RED ends on time.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
The CPU would be stuck in the loop for 5 s. Nothing else runs: the button
is not read, UART data is lost, other tasks freeze.
In the test the mock clock moves only when the test advances it. A loop
waiting for the clock would never end, so the test hangs. The test also
checks that update returns at once when no time passed.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
With "now" every late update adds its delay, so the cycles slowly drift
and get longer. With "start + duration" the schedule stays exact and
there is no drift, but the next phase gets shorter by the delay.
For a traffic light "now" is safer, because every phase gets its full
time. I would use "start + duration" when a fixed rate matters, like a
clock or sampling a sensor every 10 ms.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
traffic_light.c does not change at all. I only need to:
- write real `write` and `read` functions for the chip's GPIO registers
  (and set the pins as outputs and the button as input),
- write `now_ms` that returns a counter incremented by a 1 ms timer
  interrupt (for example SysTick),
- in main, fill the hal structs, call tl_init, and call tl_update in the
  main loop.