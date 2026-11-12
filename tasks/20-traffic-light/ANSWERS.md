# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
Suppose `start = 0xFFFFF000` (4096 ms before wrap-around) and `TL_RED_MS = 5000`; `start + 5000` overflows 32 bits and wraps around to `0x00000388` (904). If `now` is `0xFFFFF001` (only 1 ms later), `now >= 904` is immediately true, ending the phase 4999 ms early. Using unsigned subtraction `(uint32_t)(now - start)` yields `0xFFFFF001 - 0xFFFFF000 = 1`, and even after `now` wraps to `0x00000388`, modular arithmetic gives `0x00000388 - 0xFFFFF000 = 5000`, accurately measuring elapsed time across the boundary.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
In a cooperative superloop without an OS, a 5-second blocking delay inside `tl_update` freezes the CPU so no other tasks (UART, motor control, sensor polling, or even checking the pedestrian button during RED) can run until the delay finishes. The unit test catches blocking behavior by calling `tl_update` with a mock clock whose time only advances when the test explicitly steps it, so a loop waiting for `now_ms()` to change would hang or read the clock repeatedly instead of returning immediately after one check.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Setting `phase_start_ms = now` guarantees that the new phase gets its full minimum duration from the moment the lamps actually switch, which causes overall cycle times to drift slightly later whenever `tl_update` is delayed. Anchoring to `start + duration` prevents cumulative phase drift over many cycles, which is preferable for periodic data acquisition, audio sampling, or synchronized communication schedules where maintaining an exact long-term cadence matters more than stretching a single delayed interval.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
Nothing inside `traffic_light.c` or `traffic_light.h` changes at all. On the real microcontroller, you only provide concrete implementations of `hal_gpio_t` (writing to GPIO output registers and reading input registers) and `hal_clock_t` (returning a hardware timer or SysTick millisecond counter) and pass pointers to those structs into `tl_init`.