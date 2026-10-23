/*
 * traffic_light.h - a non-blocking traffic light controller over the HAL.
 *
 * Sequence (European style), with the lamps that are on:
 *
 *   RED (5 s)  ->  RED_YELLOW (1 s)  ->  GREEN (4 s)  ->  YELLOW (2 s)  -> RED ...
 *   red            red + yellow          green            yellow
 *
 * A pedestrian button shortens GREEN: once the button has been seen pressed
 * (in any phase), GREEN ends as soon as it has lasted TL_GREEN_MIN_MS.
 *
 * NON-BLOCKING: the main loop calls tl_update() as often as it likes
 * (thousands of times per second). Each call looks at the clock, does at
 * most one phase change, and returns immediately. It never waits, so the
 * same loop can also run a display, a UART, other state machines...
 *
 *     while (1) {
 *         tl_update(&tl);
 *         uart_poll();
 *         ...
 *     }
 *
 * All hardware access goes through hal_gpio_t and hal_clock_t. On a board,
 * pass real implementations of those and this file works unchanged.
 *
 * Do not change this file.
 */
#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <stdbool.h>
#include <stdint.h>

#include "hal_clock.h"
#include "hal_gpio.h"

/* Pins (all active high: GPIO_HIGH = lamp on / button pressed). */
#define TL_PIN_RED     0u
#define TL_PIN_YELLOW  1u
#define TL_PIN_GREEN   2u
#define TL_PIN_BUTTON  3u

/* Phase durations in milliseconds. */
#define TL_RED_MS         5000u
#define TL_RED_YELLOW_MS  1000u
#define TL_GREEN_MS       4000u
#define TL_GREEN_MIN_MS   1000u
#define TL_YELLOW_MS      2000u

typedef enum {
    TL_RED = 0,
    TL_RED_YELLOW,
    TL_GREEN,
    TL_YELLOW
} tl_phase_t;

typedef struct {
    const hal_gpio_t *gpio;
    const hal_clock_t *clock;
    tl_phase_t phase;
    uint32_t phase_start_ms;  /* clock value when the current phase began */
    bool ped_request;         /* button seen pressed, not served yet */
} traffic_light_t;

/*
 * Start in RED at the current time, with the lamps set accordingly and no
 * pedestrian request. Returns false and does nothing if any pointer is NULL
 * or the interfaces lack a function (gpio->write, gpio->read, clock->now_ms).
 */
bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock);

/*
 * Do one step:
 *   1. read the clock once
 *   2. read the button; if pressed, remember the request
 *   3. if the current phase is over, switch to the next one:
 *        - normal end: the phase has lasted its full duration
 *        - GREEN also ends early when a request is pending and it has
 *          lasted at least TL_GREEN_MIN_MS
 *      the new phase starts "now" (the time read in step 1), the lamps are
 *      set for it, and entering YELLOW clears the pedestrian request
 *   At most one phase change per call. Must work when the millisecond
 *   counter wraps around from 0xFFFFFFFF to 0. NULL: nothing.
 */
void tl_update(traffic_light_t *tl);

/* Current phase. NULL -> TL_RED. */
tl_phase_t tl_phase(const traffic_light_t *tl);

/* Is a pedestrian request waiting? NULL -> false. */
bool tl_pedestrian_waiting(const traffic_light_t *tl);

#endif /* TRAFFIC_LIGHT_H */
