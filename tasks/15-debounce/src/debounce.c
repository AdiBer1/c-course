#include "debounce.h"

#include <stddef.h>

void debounce_init(debounce_t *d, uint8_t threshold_ticks)
{
    if (d == NULL) {
        return;
    }
    d->stable = BTN_RELEASED;
    d->counter = 0;
    d->threshold = (threshold_ticks == 0) ? 1 : threshold_ticks;
    d->presses = 0;
}

btn_event_t debounce_update(debounce_t *d, bool raw_pressed)
{
    if (d == NULL) {
        return BTN_EVENT_NONE;
    }

    bool stable_pressed = (d->stable == BTN_PRESSED);

    if (raw_pressed == stable_pressed) {
        d->counter = 0;
        return BTN_EVENT_NONE;
    }

    d->counter = (uint8_t)(d->counter + 1u);
    if (d->counter < d->threshold) {
        return BTN_EVENT_NONE;
    }

    d->counter = 0;
    if (raw_pressed) {
        d->stable = BTN_PRESSED;
        d->presses++;
        return BTN_EVENT_PRESS;
    }
    d->stable = BTN_RELEASED;
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
        return 0;
    }
    return d->presses;
}