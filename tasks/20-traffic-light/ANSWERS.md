# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
If start is 0xFFFFF000 (4,294,963,200) and TL_RED_MS is 5000, start + 5000 overflows 32 bits and wraps to 904, making now >= 904 prematurely true. Unsigned subtraction (uint32_t)(now - start) uses modulo $2^{32}$ arithmetic: when now wraps to 1000, (uint32_t)(1000 - 0xFFFFF000) yields the exact elapsed time of 5096 ms, which properly satisfies >= 5000.
## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
A blocking delay freezes the entire main loop for 5 seconds, starving all other tasks, sensors, and communications of CPU time. The test catches this by calling tl_update once without advancing the mock clock and verifying that it returns immediately without hanging or modifying time.
## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Anchoring to now causes cumulative drift (each late update permanently shifts subsequent transitions forward), whereas start + duration eliminates drift and preserves long-term frequency. Use now when minimum state duration is critical (e.g., guaranteeing a full yellow-light safety interval), and start + duration when strict periodic accuracy is required (e.g., clocks or data sampling).
## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
The traffic light logic remains identical; only the HAL backend changes. You replace the mock hal_gpio_t functions with hardware GPIO register writes, replace the mock hal_clock_t with a hardware timer or SysTick interrupt counter, and add peripheral/clock initialization in main().
