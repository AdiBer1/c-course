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
    if (d == NULL) {
        return BTN_EVENT_NONE;
    }

    btn_state_t raw = raw_pressed ? BTN_PRESSED : BTN_RELEASED;

    /* Input agrees with the stable state: any bounce is over, start again. */
    if (raw == d->stable) {
        d->counter = 0u;
        return BTN_EVENT_NONE;
    }

    /* Input disagrees: count how many ticks IN A ROW it has disagreed. */
    d->counter++;
    if (d->counter < d->threshold) {
        return BTN_EVENT_NONE; /* not long enough yet: maybe just a bounce */
    }

    /* Disagreed for `threshold` ticks in a row: accept the change.
     * The event is returned on this ONE tick only. */
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
    return (d == NULL) ? BTN_RELEASED : d->stable;
}

uint32_t debounce_press_count(const debounce_t *d)
{
    return (d == NULL) ? 0u : d->presses;
}