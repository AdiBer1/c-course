#include <stddef.h>
#include "debounce.h"

/* Initialize debouncer to released state with minimum threshold of 1 tick */
void debounce_init(debounce_t *d, uint8_t threshold_ticks)
{
    if (d == NULL) {
        return;
    }
    d->stable = BTN_RELEASED;
    d->counter = 0;
    d->threshold = (threshold_ticks == 0) ? 1 : threshold_ticks; /* 0 is treated as 1 */
    d->presses = 0;
}

/* Process one tick of raw input and return a press/release event once stable */
btn_event_t debounce_update(debounce_t *d, bool raw_pressed)
{
    if (d == NULL) {
        return BTN_EVENT_NONE;
    }

    btn_state_t raw = raw_pressed ? BTN_PRESSED : BTN_RELEASED;

    /* Input matches stable state: reset bounce counter */
    if (raw == d->stable) {
        d->counter = 0;
        return BTN_EVENT_NONE;
    }

    /* Count consecutive ticks with the new state until threshold is met */
    d->counter++;
    if (d->counter < d->threshold) {
        return BTN_EVENT_NONE;
    }

    /* Threshold reached: commit new state and emit transition event */
    d->counter = 0;
    d->stable = raw;
    if (raw == BTN_PRESSED) {
        d->presses++;
        return BTN_EVENT_PRESS;
    }
    return BTN_EVENT_RELEASE;
}

/* Return the current debounced button state */
btn_state_t debounce_state(const debounce_t *d)
{
    return (d == NULL) ? BTN_RELEASED : d->stable;
}

/* Return total number of debounced press transitions */
uint32_t debounce_press_count(const debounce_t *d)
{
    return (d == NULL) ? 0U : d->presses;
}