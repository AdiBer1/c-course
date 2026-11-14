# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
Say RED starts at start = 0xFFFFF000 (4,294,963,200), just before the counter wraps. start + 5000 is bigger than 0xFFFFFFFF, so it wraps to 904. One millisecond later now = 0xFFFFF001, and the check "now >= 904" is already true, so RED ends after 1 ms instead of 5 seconds.
With subtraction: one ms later, now - start = 1, which is less than 5000, so RED continues. After the counter wraps, 5000 ms later now = 904, and (uint32_t)(904 - 0xFFFFF000) also wraps and gives exactly 5000. So the elapsed time is always right, as long as it is less than about 49 days. Unsigned subtraction "wraps back" the same way the counter wrapped.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
For 5 seconds the CPU would only wait. Nothing else in the main loop would run: the button would not be read, bytes from the UART would be lost because nobody takes them from the buffer, a display would freeze, and a watchdog could even reset the chip. In the test the clock is a mock, and time moves only when the test changes it. So a loop that waits for the clock would never end and the test would hang. Also `sleep`, `usleep` and `nanosleep` are forbidden symbols, so they are found by the check.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
With "now", every late update adds its delay to the cycle, so over many cycles the light slowly drifts and the cycles are a little longer than 12 seconds. But every phase always gets its full time, for example yellow is never shorter than 2 seconds, which is safer for a traffic light. With "start + duration" there is no drift, the cycles stay on a fixed schedule, but after a late update the next phase is shorter to catch up. I would use "start + duration" when the rhythm matters more, like a clock, sampling a sensor at a fixed rate, or LEDs that need to blink in sync.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
traffic_light.c doesn't change at all. I need to write:
- a real `hal_gpio_t`: init that turns on the port clock and sets pins 0, 1, 2 as outputs and pin 3 as input (with a pull-down, because the button is active high), and write/read functions that use the GPIO registers.
- a real `hal_clock_t`: a timer, for example SysTick, that makes an interrupt every 1 ms and increments a `volatile uint32_t` counter, and `now_ms` returns this counter.
- main: init the two HALs, call `tl_init`, and then call `tl_update` in the main loop.
If the pins on the board are different, only the HAL maps them. On an 8-bit MCU, reading a 32-bit counter is not one step, so `now_ms` should turn off interrupts for a moment while it reads it.