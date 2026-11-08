# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
Say RED started at start = 4,294,965,000, 2,296 ms before the wrap. Then start + 5000 overflows and becomes 2,704. One millisecond later now = 4,294,965,001, which is bigger than 2,704, so RED ends after 1 ms instead of 5 s. With subtraction, now - start is 1, so we keep waiting. After the wrap, at now = 2,704, now - start is exactly 5,000, so RED ends at the right moment. The unsigned subtraction “goes around” the wrap like a clock face.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
The whole CPU would be stuck for 5 seconds. Buttons wouldn’t be read, UART bytes would be lost, other LEDs would freeze, and a watchdog might even reset the board. In the test the clock is a mock that only moves when the test moves it, so a loop waiting for the clock would never end: the test would hang and fail. (sleep, usleep and nanosleep are also checked as forbidden symbols.)

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Starting at “now” means every late update makes the cycle a bit longer, so over many cycles the light drifts. For example, 300 ms late in each of the 4 phases adds 1.2 s to every 12 s cycle. But no phase is ever shorter than its duration, which matters for safety: yellow is never cut short. Starting at “start + duration” keeps the exact average rhythm with no drift, but the next phase gets shorter by the lateness. I’d prefer that for periodic jobs that need a fixed rate, like sampling a sensor every 10 ms or keeping several lights in sync.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
traffic_light.c and its header don’t change at all. I only need to write real HAL implementations: a write function that sets the output pins in the port registers, a read function that reads the button pin, and a now_ms function that returns a millisecond counter updated by a timer interrupt (like SysTick). Then the board’s startup code configures the pins and the timer, fills the two HAL structs, calls tl_init, and calls tl_update in the main loop.
