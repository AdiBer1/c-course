#include "debounce.h"

#include <stddef.h>

void debounce_init(debounce_t *d, uint8_t threshold_ticks)
{
    if (!d) return;
    d->stable    = BTN_RELEASED;
    d->counter   = 0;
    d->threshold = threshold_ticks == 0 ? 1 : threshold_ticks;
    d->presses   = 0;
}

btn_event_t debounce_update(debounce_t *d, bool raw_pressed)
{
    if (!d) return BTN_EVENT_NONE;
    bool stable_pressed = (d->stable == BTN_PRESSED);
    if (raw_pressed != stable_pressed) {
        d->counter++;
        if (d->counter >= d->threshold) {
            d->counter = 0;
            d->stable  = raw_pressed ? BTN_PRESSED : BTN_RELEASED;
            if (d->stable == BTN_PRESSED) {
                d->presses++;
                return BTN_EVENT_PRESS;
            }
            return BTN_EVENT_RELEASE;
        }
    } else {
        d->counter = 0;
    }
    return BTN_EVENT_NONE;
}

btn_state_t debounce_state(const debounce_t *d)
{
    return d ? d->stable : BTN_RELEASED;
}

uint32_t debounce_press_count(const debounce_t *d)
{
    return d ? d->presses : 0;
}
