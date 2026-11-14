# Answers - Task 20: Non-blocking traffic light

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer

When `now` approaches the 32-bit integer limit (`0xFFFFFFFF`) and wraps around to `0`, adding `TL_RED_MS` (e.g., `5000`) to `phase_start_ms` (e.g., `0xFFFFFFFE`) causes an arithmetic overflow, wrapping the expected target time to a small number like `4998`. Since `now` (say, `10`) is much smaller than `4998`, the direct comparison `now >= target` evaluates to false prematurely, freezing the state machine. In contrast, unsigned subtraction `(uint32_t)(now - start)` exploits modular arithmetic properties in C: even across a wrap-around, subtracting `0xFFFFFFFE` from `10` yields the correct positive elapsed time (`12`), making the comparison robust and correct.


## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer

If `tl_update` used a blocking delay loop, the entire CPU would be trapped inside that function for the entire duration of the phase. This would prevent the main loop from executing other concurrent tasks, such as handling UART communication, updating displays, or reading other sensors. The test suite catches this by running with strict execution timeouts and mock verification, detecting that control did not return immediately and that other scheduled periodic tasks missed their timing windows.


## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer

Setting the new phase start time to `now` prevents temporal drift: even if an update cycle is delayed by 300 ms due to heavy CPU load, the current phase length is preserved, and the system resynchronizes immediately. If we used `start + duration`, any delay would accumulate over many cycles, causing the traffic light timing to drift significantly relative to real wall-clock time. However, if precise absolute periodic scheduling is required (such as generating a strict 1.000 ms audio or sampling clock), anchoring to the expected target time is preferred to maintain long-term frequency accuracy.


## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer

When moving the module to a real microcontroller, the core logic in `traffic_light.c` and `traffic_light.h` remains completely unchanged. The only things that change are the initialization and implementation of the HAL structs: `hal_clock` is backed by a real hardware timer or SysTick interrupt counter instead of the mock clock, and `hal_gpio` is backed by actual memory-mapped hardware registers (like GPIO port data and direction registers) instead of the mock GPIO buffer.