# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
When start is close to the 32-bit maximum (e.g., 0xFFFFFF00), adding 5000 overflows and wraps around to a small number (4744). Then now >= 4744 becomes true immediately (0xFFFFFF00 >= 4744) before any time has passed. With unsigned subtraction (uint32_t)(now - start), even after the clock wraps around to now = 4744, the subtraction wraps cleanly and gives the exact elapsed time (5000), so the condition triggers at the right time.
## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
On a single-CPU system without an OS, a delay loop blocks the entire firmware for 5 seconds, so no other tasks (like reading buttons or UART) can run. The test catches this because it uses a mock clock that only moves forward when the test code advances it—if tl_update loops waiting for the clock, the clock will never advance and the test will hang forever.
## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Starting at now causes delays to add up over many cycles (drift), making the overall cycles longer, while guaranteeing each phase lasts at least its full duration. Starting at start + duration absorbs the delay so there is no drift over time, which is better when exact long-term timing or frequency matters (like a real-time clock or periodic sensor sampling).
## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
traffic_light.c and traffic_light.h do not change at all. What changes is the platform setup code: instead of passing test mocks to tl_init, we write real hardware functions for hal_gpio_t (setting pin voltages via GPIO registers to turn physical LEDs on/off and reading the physical button) and hal_clock_t (reading a hardware millisecond timer like SysTick), and configure the microcontroller's GPIO pin directions at startup.
