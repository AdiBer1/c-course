#include "traffic_light.h"

#include <stddef.h>

static void set_lamps(const traffic_light_t *tl)
{
    bool red = (tl->phase == TL_RED || tl->phase == TL_RED_YELLOW);
    bool yellow = (tl->phase == TL_RED_YELLOW || tl->phase == TL_YELLOW);
    bool green = (tl->phase == TL_GREEN);

    tl->gpio->write(tl->gpio->ctx, TL_PIN_RED, red ? GPIO_HIGH : GPIO_LOW);
    tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, yellow ? GPIO_HIGH : GPIO_LOW);
    tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN, green ? GPIO_HIGH : GPIO_LOW);
}

bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock)
{
    if (tl == NULL || gpio == NULL || clock == NULL || gpio->write == NULL || gpio->read == NULL
        || clock->now_ms == NULL)
        return false;

    tl->gpio = gpio;
    tl->clock = clock;

    tl->phase = TL_RED;
    tl->phase_start_ms = clock->now_ms(clock->ctx);
    tl->ped_request = false;
    set_lamps(tl);

    return true;
}

void tl_update(traffic_light_t *tl)
{
    if (!tl || !tl->gpio || !tl->clock || !tl->gpio->read || !tl->gpio->write
        || !tl->clock->now_ms) {
        return;
    }

    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);
    bool expired = false;

    switch (tl->phase) {
    case TL_RED:
        expired = (elapsed >= TL_RED_MS);
        break;
    case TL_RED_YELLOW:
        expired = (elapsed >= TL_RED_YELLOW_MS);
        break;
    case TL_GREEN:
        expired = (elapsed >= TL_GREEN_MS) || (tl->ped_request && elapsed >= TL_GREEN_MIN_MS);
        break;
    case TL_YELLOW:
        expired = (elapsed >= TL_YELLOW_MS);
        break;
    }

    if (!expired) {
        return;
    }

    switch (tl->phase) {
    case TL_RED:
        tl->phase = TL_RED_YELLOW;
        break;
    case TL_RED_YELLOW:
        tl->phase = TL_GREEN;
        break;
    case TL_GREEN:
        tl->phase = TL_YELLOW;
        tl->ped_request = false;
        break;
    case TL_YELLOW:
        tl->phase = TL_RED;
        break;
    }
    tl->phase_start_ms = now;
    set_lamps(tl);
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