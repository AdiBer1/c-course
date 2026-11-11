#include "debounce.h"

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

    btn_state_t target_state = raw_pressed ? BTN_PRESSED : BTN_RELEASED;

    if (target_state != d->stable) {
        d->counter++;
        if (d->counter >= d->threshold) {
            d->stable = target_state;
            d->counter = 0;
            if (d->stable == BTN_PRESSED) {
                d->presses++;
                return BTN_EVENT_PRESS;
            } else {
                return BTN_EVENT_RELEASE;
            }
        }
    } else {
        d->counter = 0;
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
        return 0;
    }
    return d->presses;
}