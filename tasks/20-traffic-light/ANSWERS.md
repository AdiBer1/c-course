# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
If `phase_start_ms = 0xFFFFFF00` and `TL_RED_MS = 5000`, the addition `phase_start_ms + 5000` overflows in 32-bit unsigned arithmetic to `4744`. Immediately on the next millisecond (`now = 0xFFFFFF01`), the condition `0xFFFFFF01 >= 4744` becomes true instantly, prematurely ending the phase after just 1 ms. Conversely, `(uint32_t)(now - start)` uses modular unsigned subtraction: when `now` rolls over to `4744`, `(uint32_t)(4744 - 0xFFFFFF00)` correctly yields `5000`, accurately measuring elapsed time across the wrap-around boundary.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
A delay loop blocks the CPU execution entirely, preventing the firmware from performing any other non-blocking tasks such as processing inputs, handling communication, or refreshing safety watchdogs. The test suite catches blocking behavior by testing zero-time updates (`test_update_without_time_passing_returns_immediately`) and verifying that `tl_update` executes instantly and returns control without halting caller execution.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Setting `phase_start_ms = now` resets the phase origin to the current execution timestamp, accumulating timing drift over multiple cycles if updates are delayed. Setting `phase_start_ms = start + duration` eliminates drift by maintaining a fixed overall periodic cycle. We prefer `phase_start_ms = now` when every state requires a guaranteed full minimum operational duration, whereas `start + duration` is preferred for strictly periodic, drift-free scheduling (e.g., RTC ticking or periodic sensor sampling).

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
The code in `traffic_light.c` and `traffic_light.h` requires zero changes. To deploy on a physical microcontroller, we only need to provide concrete HAL implementations for `hal_gpio_t` (reading/writing physical GPIO hardware registers) and `hal_clock_t` (reading a system tick counter like SysTick or `HAL_GetTick()`), and pass pointers to those real hardware instances into `tl_init`.

