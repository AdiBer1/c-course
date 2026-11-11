
#include "debounce.h"

#include <stddef.h>

void debounce_init(debounce_t *d, uint8_t threshold_ticks)
{
    if (d == NULL) {
        return;
    }

    d->stable = BTN_RELEASED;
    d->counter = 0U;
    d->threshold = (threshold_ticks == 0U) ? 1U : threshold_ticks;
    d->presses = 0U;
}

btn_event_t debounce_update(debounce_t *d, bool raw_pressed)
{
    if (d == NULL) {
        return BTN_EVENT_NONE;
    }

    /* Raw input matches the stable state: reset the counter. */
    if ((raw_pressed && (d->stable == BTN_PRESSED)) ||
        (!raw_pressed && (d->stable == BTN_RELEASED))) {
        d->counter = 0U;
        return BTN_EVENT_NONE;
    }

    /* Raw input differs from stable state. */
    if (d->counter < d->threshold) {
        d->counter++;
    }

    /* Accept the state change after enough consecutive ticks. */
    if (d->counter >= d->threshold) {
        d->stable = raw_pressed ? BTN_PRESSED : BTN_RELEASED;
        d->counter = 0U;

        if (d->stable == BTN_PRESSED) {
            d->presses++;
            return BTN_EVENT_PRESS;
        }

        return BTN_EVENT_RELEASE;
    }

    return BTN_EVENT_NONE;
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
        return 0U;
    }

    return d->presses;
}
