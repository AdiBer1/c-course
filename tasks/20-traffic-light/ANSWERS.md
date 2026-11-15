# Task 20 — Understanding questions

Answer in your own words, a few sentences each. Write under each `Answer:` line.

## Q1. Why is `if (now >= tl->phase_start_ms + TL_RED_MS)` wrong near the 32-bit wrap-around, while `if ((uint32_t)(now - start) >= TL_RED_MS)` is right? Use concrete numbers.

Answer:
Because in the first expression there is an addition between the arguments, and the result of the addition is stored in `uint32_t` and can wrap around. Then the end time becomes a small number, and the comparison gives a wrong answer. In the second expression there is a subtraction between the two arguments. It can also wrap around, but in `uint32_t` subtraction the wrap-around gives exactly the correct difference, which is how much time has passed.

Example: `start = 4294965296` (2000 ms before the wrap-around).
- **First expression:** `start + 5000` wraps to `3000`, so after only 1 second `now >= 3000` is already true. Red ends too early.
- **Second expression:** after the wrap, when `now = 3000`, `(uint32_t)(3000 - start) = 5000`. Red ends at the correct time.

## Q2. What would happen to the rest of the firmware if `tl_update` used a delay loop to wait for the 5 seconds of RED? How does the test catch it?

Answer:
There will be blocking: the system will get stuck, everyone will wait until the waiting is over, the whole controller will be busy, and nothing can be done during this time. For example, data that arrives on the UART will be lost, and a button press will not be read.

The test catches it because in the tests the clock is a mock, and its time moves only when the test advances it. If tl_update waits in a loop, nobody advances the clock, so the loop never ends. The test has a time limit of 5 seconds, so it is stopped and fails.

## Q3. When an update comes 300 ms late, your new phase starts at "now", not at "start + duration". What is the difference over many cycles, and when would you prefer the other choice?

Answer:
If every time there is a delay we start the new phase from now, the delays will add up and can become a significant amount of time, and the traffic light will no longer be in sync with its planned times. If instead we ignore the delay and treat the phase as if it started on time (start + duration), the delays do not add up, but the next phase becomes shorter. For example, the green light for cars will turn off after 4 seconds according to the system’s count, while in reality it turned on only, say, 1 second before.

When is it better to use the second option: in a system that must keep an exact rhythm, like a clock that shows the time. Not in a system like a traffic light, where it does not matter so much exactly when a phase starts and ends, only that each phase gets its full time according to the system settings.

## Q4. This module only uses `hal_gpio_t` and `hal_clock_t`. List exactly what changes when you move it to a real microcontroller.

Answer:
We only need to change the part that talks to the hardware. The file traffic_light.c stays the same. Instead of the test mock, we write real write and read functions that turn on the lamps and read the button on the board itself. We also write a real now_ms that returns how many milliseconds have passed, using a timer on the board. We connect the lamps and the button to the right pins. And in main we connect everything: we call tl_init once, and then call tl_update in an endless loop.
