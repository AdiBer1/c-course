/*
 * debounce.h - turn a bouncing button signal into clean press/release events.
 *
 * A mechanical button does not switch cleanly: for a few milliseconds the
 * contact bounces and the input reads 1 0 1 1 0 1 ... Without debouncing,
 * one press can count as five.
 *
 * This debouncer is a small STATE MACHINE driven by a periodic TICK
 * (for example a 1 ms timer interrupt calls debounce_update()):
 *
 *   - it remembers the last STABLE state (pressed or released)
 *   - each tick, if the raw input differs from the stable state, a counter
 *     goes up; if the raw input equals the stable state, the counter resets
 *   - when the counter reaches `threshold`, the stable state flips and the
 *     tick returns an event (PRESS or RELEASE); the counter resets
 *
 * So with threshold 5 the input must disagree with the stable state for 5
 * ticks IN A ROW before anything happens. Bounces shorter than that are
 * ignored. The code never waits or sleeps: it does a tiny bit of work per
 * tick and returns immediately.
 *
 * Do not change this file.
 */
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    BTN_RELEASED = 0,
    BTN_PRESSED
} btn_state_t;

typedef enum {
    BTN_EVENT_NONE = 0,
    BTN_EVENT_PRESS,
    BTN_EVENT_RELEASE
} btn_event_t;

typedef struct {
    btn_state_t stable;  /* debounced state */
    uint8_t counter;     /* consecutive ticks the raw input disagreed */
    uint8_t threshold;   /* ticks needed to accept a change (>= 1) */
    uint32_t presses;    /* number of PRESS events so far */
} debounce_t;

/*
 * Start in BTN_RELEASED with no presses counted.
 * threshold_ticks == 0 is treated as 1. Does nothing if d is NULL.
 */
void debounce_init(debounce_t *d, uint8_t threshold_ticks);

/* One tick with the current raw input (true = contact closed). NULL -> NONE. */
btn_event_t debounce_update(debounce_t *d, bool raw_pressed);

/* Current debounced state. NULL -> BTN_RELEASED. */
btn_state_t debounce_state(const debounce_t *d);

/* Number of PRESS events since init. NULL -> 0. */
uint32_t debounce_press_count(const debounce_t *d);

#endif /* DEBOUNCE_H */
