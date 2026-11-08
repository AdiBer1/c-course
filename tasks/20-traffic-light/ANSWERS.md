# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
The clock is a 32-bit counter, so it wraps around to zero after reaching its maximum value. For example, if start is 4294965295 and 100 milliseconds pass, now becomes 4294965395. Adding 5000 to start overflows and gives 2999, so the comparison incorrectly says that 5000 milliseconds have passed. However, subtracting start from now gives 100, which is the correct elapsed time.
## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
A delay loop would block the CPU for 5 seconds and prevent other tasks from running. This could delay reading sensors or handling communication. The test calls tl_update many times without advancing the mock clock. If the function waits in a loop, the test hangs instead of returning.
## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Starting the new phase at now means that every phase gets its full duration, but the whole traffic light cycle may become slightly longer if updates are late. Starting at start plus duration keeps the schedule closer to the original timing, but a phase may become shorter after a late update. Starting at now is useful when every phase needs its full duration.
## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
The traffic light logic does not need to change. Only the HAL implementations need to change: the GPIO functions must control the real hardware pins, and the clock function must return the real millisecond counter. The traffic light code can stay the same.
