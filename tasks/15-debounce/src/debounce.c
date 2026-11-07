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
    btn_state_t raw;

    if (d == NULL) {
        return BTN_EVENT_NONE;
    }

    raw = raw_pressed ? BTN_PRESSED : BTN_RELEASED;

    /* raw input agrees with the stable state: any bounce is forgotten */
    if (raw == d->stable) {
        d->counter = 0u;
        return BTN_EVENT_NONE;
    }

    /* raw input disagrees: count one more tick in a row */
    d->counter = (uint8_t)(d->counter + 1u);

    if (d->counter < d->threshold) {
        return BTN_EVENT_NONE; /* not stable long enough yet */
    }

    /* disagreed for `threshold` ticks in a row: accept the change */
    d->counter = 0u;
    d->stable = raw;

    if (raw == BTN_PRESSED) {
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