#include "traffic_light.h"

#include <stddef.h>

/* TODO: implement. Never wait or loop until time has passed! */

bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock)
{
    (void)tl;
    (void)gpio;
    (void)clock;
    return false;
}

void tl_update(traffic_light_t *tl)
{
    (void)tl;
}

tl_phase_t tl_phase(const traffic_light_t *tl)
{
    (void)tl;
    return TL_RED;
}

bool tl_pedestrian_waiting(const traffic_light_t *tl)
{
    (void)tl;
    return false;
}
