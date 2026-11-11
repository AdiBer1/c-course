#include "debounce.h"

#include <stddef.h>

void debounce_init(debounce_t *d, uint8_t threshold_ticks)
{
    if (d == NULL) {
        return;
    }

    d->stable = BTN_RELEASED;
    d->counter = 0u;
    d->threshold = (threshold_ticks == 0u) ? 1u : threshold_ticks;
    d->presses = 0u;
}

btn_event_t debounce_update(debounce_t *d, bool raw_pressed)
{
    btn_state_t raw_state;

    if (d == NULL) {
        return BTN_EVENT_NONE;
    }

    raw_state = raw_pressed ? BTN_PRESSED : BTN_RELEASED;

    if (raw_state == d->stable) {
        /* input agrees with the stable state: it was only a bounce, start over */
        d->counter = 0u;
        return BTN_EVENT_NONE;
    }

    d->counter++;

    if (d->counter < d->threshold) {
        return BTN_EVENT_NONE;
    }

    /* the input disagreed for `threshold` ticks in a row: accept the change */
    d->counter = 0u;
    d->stable = raw_state;

    if (raw_state == BTN_PRESSED) {
        d->presses++;
        return BTN_EVENT_PRESS;
    }

    return BTN_EVENT_RELEASE;
}

btn_state_t debounce_state(const debounce_t *d)
{
    if (d == NULL) {
        return BTN_RELEASED;
    }
    return d->stable;
}

uint32_t debounce_press_count(const debounce_t *d)
{
    if (d == NULL) {
        return 0u;
    }
    return d->presses;
}