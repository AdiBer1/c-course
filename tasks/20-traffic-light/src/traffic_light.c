#include "traffic_light.h"

#include <stddef.h>

static void update_lamps(const hal_gpio_t *gpio, tl_phase_t phase)
{
    bool red_on = false;
    bool yellow_on = false;
    bool green_on = false;

    switch (phase) {
        case TL_RED:
            red_on = true;
            break;
        case TL_RED_YELLOW:
            red_on = true;
            yellow_on = true;
            break;
        case TL_GREEN:
            green_on = true;
            break;
        case TL_YELLOW:
            yellow_on = true;
            break;
    }

    gpio->write(gpio->ctx, TL_PIN_RED, red_on ? GPIO_HIGH : GPIO_LOW);
    gpio->write(gpio->ctx, TL_PIN_YELLOW, yellow_on ? GPIO_HIGH : GPIO_LOW);
    gpio->write(gpio->ctx, TL_PIN_GREEN, green_on ? GPIO_HIGH : GPIO_LOW);
}

bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock)
{
    if (tl == NULL || gpio == NULL || clock == NULL) {
        return false;
    }
    if (gpio->write == NULL || gpio->read == NULL || clock->now_ms == NULL) {
        return false;
    }

    tl->gpio = gpio;
    tl->clock = clock;
    tl->phase = TL_RED;
    tl->phase_start_ms = clock->now_ms(clock->ctx);
    tl->ped_request = false;

    update_lamps(tl->gpio, tl->phase);

    return true;
}

void tl_update(traffic_light_t *tl)
{
    if (tl == NULL || tl->gpio == NULL || tl->clock == NULL) {
        return;
    }
    if (tl->gpio->write == NULL || tl->gpio->read == NULL || tl->clock->now_ms == NULL) {
        return;
    }

    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);
    bool phase_over = false;

    switch (tl->phase) {
        case TL_RED:
            if (elapsed >= TL_RED_MS) {
                phase_over = true;
            }
            break;
        case TL_RED_YELLOW:
            if (elapsed >= TL_RED_YELLOW_MS) {
                phase_over = true;
            }
            break;
        case TL_GREEN:
            if (elapsed >= TL_GREEN_MS || (tl->ped_request && elapsed >= TL_GREEN_MIN_MS)) {
                phase_over = true;
            }
            break;
        case TL_YELLOW:
            if (elapsed >= TL_YELLOW_MS) {
                phase_over = true;
            }
            break;
    }

    if (phase_over) {
        switch (tl->phase) {
            case TL_RED:
                tl->phase = TL_RED_YELLOW;
                break;
            case TL_RED_YELLOW:
                tl->phase = TL_GREEN;
                break;
            case TL_GREEN:
                tl->phase = TL_YELLOW;
                break;
            case TL_YELLOW:
                tl->phase = TL_RED;
                break;
        }

        tl->phase_start_ms = now;

        if (tl->phase == TL_YELLOW) {
            tl->ped_request = false;
        }

        update_lamps(tl->gpio, tl->phase);
    }
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