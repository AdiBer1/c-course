# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
With start = 0xFFFFFC00 and duration = 5000, start + duration wraps to 3904, so the condition is true after only 16ms. With (uint32_t)(now - start) the subtraction is done modulo 2^32, so the result is 16 even after the counter wraps.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
A delay loop would block the CPU for 5 seconds, so the firmware could not read buttons, handle UART communication, or run other state machines. The test uses a mock clock that only advances when the test code explicitly increments it, so a loop waiting for time to pass would hang forever (causing the test to hit its 5-second timeout).

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Starting at now makes each late update extend the cycle, so the error accumulates (drifts) over many cycles. Using start + duration avoids cumulative timing drift, and I would prefer it when a fixed, accurate long-term frequency is required (such as periodic sensor sampling, slow PWM, or communication protocols)—whereas starting at now is better when safety requires each state to last at least its full minimum duration (like a yellow traffic light).


## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
Only the HAL (Hardware Abstraction Layer) implementation and hardware initialization changes: a real write/read that access GPIO registers, and a now_ms that reads a hardware timer counter incremented by an interrupt. The traffic light code itself stays completely unchanged.

