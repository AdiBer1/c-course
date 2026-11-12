# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
When `phase_start_ms` is near `0xFFFFFFFF` (e.g., `4,294,967,000`), adding `5000` causes an unsigned integer overflow wrapping to `4031`. The condition `now >= 4031` evaluates to true immediately when `now` is still `4,294,967,000`, causing premature phase termination. Subtracting `(uint32_t)(now - start)` uses 2's complement modular arithmetic, which correctly yields the elapsed time `(50 - 4,294,967,000) = 5000` regardless of wrap-around.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
A busy-wait delay loop blocks the CPU core, preventing execution of all other concurrent tasks in the main loop (such as reading UART, updating displays, or responding to interrupts). The unit tests catch blocking behavior by checking call counts or clock step progress to verify that `tl_update()` returns execution to the caller immediately within a single time check.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
Starting at "now" absorbs execution delays and avoids phase truncation, but introduces phase drift over multiple cycles. Using "start + duration" preserves a fixed periodic schedule over long durations. "Start + duration" is preferred in time-critical periodic tasks (like real-time clocks or fixed PWM generators), whereas "now" is better for event-driven physical displays like traffic lights where minimum phase guarantees matter.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
Zero code inside `traffic_light.c` or `traffic_light.h` changes. You only need to instantiate concrete hardware implementations for `hal_gpio_t` (mapping `write` and `read` to physical GPIO registers) and `hal_clock_t` (mapping `now_ms` to a hardware timer or SysTick millisecond counter), and pass their pointers to `tl_init()`.