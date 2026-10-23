#include "debounce.h"

#include <stddef.h>

/* TODO: implement the state machine. */

void debounce_init(debounce_t *d, uint8_t threshold_ticks)
{
    (void)d;
    (void)threshold_ticks;
}

btn_event_t debounce_update(debounce_t *d, bool raw_pressed)
{
    (void)d;
    (void)raw_pressed;
    return BTN_EVENT_NONE;
}

btn_state_t debounce_state(const debounce_t *d)
{
    (void)d;
    return BTN_RELEASED;
}

uint32_t debounce_press_count(const debounce_t *d)
{
    (void)d;
    return 0;
}
