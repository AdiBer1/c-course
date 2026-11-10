#include "traffic_light.h"

#include <stddef.h>

static void write_lamp(const traffic_light_t *tl, uint8_t pin, bool on)
{
    tl->gpio->write(tl->gpio->ctx, pin, on ? GPIO_HIGH : GPIO_LOW);
}

static void set_lamps(const traffic_light_t *tl)
{
    bool red = (tl->phase == TL_RED || tl->phase == TL_RED_YELLOW);
    bool yellow = (tl->phase == TL_RED_YELLOW || tl->phase == TL_YELLOW);
    bool green = (tl->phase == TL_GREEN);

    write_lamp(tl, TL_PIN_RED, red);
    write_lamp(tl, TL_PIN_YELLOW, yellow);
    write_lamp(tl, TL_PIN_GREEN, green);
}

static void enter_phase(traffic_light_t *tl, tl_phase_t phase, uint32_t now)
{
    tl->phase = phase;
    tl->phase_start_ms = now;
    if (phase == TL_YELLOW) {
        tl->ped_request = false;
    }
    set_lamps(tl);
}

bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock)
{
    if (tl == NULL || gpio == NULL || clock == NULL || gpio->write == NULL || gpio->read == NULL
        || clock->now_ms == NULL) {
        return false;
    }
    tl->gpio = gpio;
    tl->clock = clock;
    tl->ped_request = false;
    enter_phase(tl, TL_RED, clock->now_ms(clock->ctx));
    return true;
}

void tl_update(traffic_light_t *tl)
{
    (void)tl;
}

tl_phase_t tl_phase(const traffic_light_t *tl)
{
    if (tl == NULL) {
        return TL_RED;
    }
    return tl->phase;
}

bool tl_pedestrian_waiting(const traffic_light_t *tl)
{
    if (tl == NULL) {
        return false;
    }
    return tl->ped_request;
}