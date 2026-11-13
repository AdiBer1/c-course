# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
Say RED starts 2000 ms before the wrap: start = 4,294,965,296 (0xFFFFF830). Then `start + 5000` = 4,294,970,296, which does not fit in 32 bits and wraps to 3000. One millisecond later now = 4,294,965,297, and `now >= 3000` is already TRUE, so RED would end after 1 ms instead of 5 s. With subtraction: right after the start, `now - start` = 1, which is less than 5000, correct. After the counter wraps, at now = 3000, `now - start` = 3000 - 4,294,965,296, which modulo 2^32 is exactly 5000, so RED ends at the right moment. Unsigned subtraction is defined modulo 2^32, so the elapsed time is always correct as long as the real interval is shorter than about 49.7 days.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
The CPU would be stuck inside `tl_update` for 5 seconds, and nothing else in the main loop would run: the UART would not be polled, so received bytes would overflow its buffer and be lost; a display would freeze; buttons would not be read; other state machines would miss their deadlines; and a watchdog could reset the whole device. In the test there is no real time: the mock clock only moves when the test changes it between calls. A loop that waits for the clock to advance would therefore never end, so the test hangs and is killed by the timeout, and the score shows the failure.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Starting at "now" means each phase lasts at least its full duration, measured from when it really began, but every late update adds its delay to the cycle. Over many cycles these delays add up, so the light slowly drifts compared to an exact 12-second schedule. Starting at "start + duration" keeps the schedule exact on average: a late phase is followed by a slightly shorter one, so there is no long-term drift, but a single phase can be shorter than its nominal time, and after a long gap the code could have to catch up several phases in a row. For a traffic light, "now" is the safe choice: a phase must never be shorter than specified (yellow must really last 2 s). "start + duration" is better when the long-term rate matters more than each single interval, for example for a clock display, periodic sampling at an exact frequency, or several lights that must stay in sync.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
`traffic_light.c` does not change at all. What has to be written for the board: (1) a real `hal_gpio_t` implementation whose `write` sets or clears the output pins connected to the three lamps and whose `read` reads the button pin, plus code that configures those pins (outputs for the lamps, input with a pull resistor for the button); (2) a real `hal_clock_t` whose `now_ms` returns a millisecond counter, usually a `volatile uint32_t` incremented by a 1 ms SysTick or timer interrupt; (3) a `main()` that initialises the clocks and peripherals, calls `tl_init` with those two structs and then calls `tl_update` in its main loop. If the board wiring differs, only the pin numbers or the HAL implementation change. And since a real button bounces, I would probably feed it through the debouncer from task 15.