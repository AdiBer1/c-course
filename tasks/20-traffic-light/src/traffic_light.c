#include "traffic_light.h"

#include <stddef.h>

bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock)
{
    if (tl == NULL || gpio == NULL || clock == NULL ||
        gpio->write == NULL || gpio->read == NULL ||
        clock->now_ms == NULL) {
        return false;
    }

    tl->gpio = gpio;
    tl->clock = clock;
    tl->phase = TL_RED;
    tl->phase_start_ms = clock->now_ms(clock->ctx);
    tl->ped_request = false;

    gpio->write(gpio->ctx, TL_PIN_RED, GPIO_HIGH);
    gpio->write(gpio->ctx, TL_PIN_YELLOW, GPIO_LOW);
    gpio->write(gpio->ctx, TL_PIN_GREEN, GPIO_LOW);

    return true;
}

void tl_update(traffic_light_t *tl)
{
    if (tl == NULL) {
        return;
    }

    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);
    tl_phase_t next_phase = tl->phase;
    bool change = false;

    switch (tl->phase) {
    case TL_RED:
        if (elapsed >= TL_RED_MS) {
            next_phase = TL_RED_YELLOW;
            change = true;
        }
        break;

    case TL_RED_YELLOW:
        if (elapsed >= TL_RED_YELLOW_MS) {
            next_phase = TL_GREEN;
            change = true;
        }
        break;

    case TL_GREEN:
        if (elapsed >= TL_GREEN_MS ||
            (tl->ped_request && elapsed >= TL_GREEN_MIN_MS)) {
            next_phase = TL_YELLOW;
            change = true;
        }
        break;

    case TL_YELLOW:
        if (elapsed >= TL_YELLOW_MS) {
            next_phase = TL_RED;
            change = true;
        }
        break;

    default:
        return;
    }

    if (!change) {
        return;
    }

    tl->phase = next_phase;
    tl->phase_start_ms = now;

    if (next_phase == TL_YELLOW) {
        tl->ped_request = false;
    }

    tl->gpio->write(tl->gpio->ctx, TL_PIN_RED,
                    (next_phase == TL_RED || next_phase == TL_RED_YELLOW)
                        ? GPIO_HIGH : GPIO_LOW);

    tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW,
                    (next_phase == TL_RED_YELLOW || next_phase == TL_YELLOW)
                        ? GPIO_HIGH : GPIO_LOW);

    tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN,
                    (next_phase == TL_GREEN) ? GPIO_HIGH : GPIO_LOW);
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