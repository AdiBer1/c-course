#include "debounce.h"

#include <stddef.h>

/* TODO: implement the state machine. */

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

    bool raw = raw_pressed;
    bool is_pressed_state = (d->stable == BTN_PRESSED);

    if (raw != is_pressed_state) {
        d->counter++;
        if (d->counter >= d->threshold) {
            d->stable = raw ? BTN_PRESSED : BTN_RELEASED;
            d->counter = 0u;
            if (d->stable == BTN_PRESSED) {
                d->presses++;
                return BTN_EVENT_PRESS;
            } else {
                return BTN_EVENT_RELEASE;
            }
        }
    } else {
        d->counter = 0u;
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
        return 0u;
    }
    return d->presses;
}