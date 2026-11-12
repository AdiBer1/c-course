#include "traffic_light.h"
#include <stddef.h>

static void set_lamps(const traffic_light_t *tl, tl_phase_t phase)
{
    if (tl == NULL || tl->gpio == NULL || tl->gpio->write == NULL) {
        return;
    }

    switch (phase) {
    case TL_RED:
        tl->gpio->write(tl->gpio->ctx, TL_PIN_RED, GPIO_HIGH);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, GPIO_LOW);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN, GPIO_LOW);
        break;
    case TL_RED_YELLOW:
        tl->gpio->write(tl->gpio->ctx, TL_PIN_RED, GPIO_HIGH);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, GPIO_HIGH);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN, GPIO_LOW);
        break;
    case TL_GREEN:
        tl->gpio->write(tl->gpio->ctx, TL_PIN_RED, GPIO_LOW);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, GPIO_LOW);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN, GPIO_HIGH);
        break;
    case TL_YELLOW:
        tl->gpio->write(tl->gpio->ctx, TL_PIN_RED, GPIO_LOW);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, GPIO_HIGH);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN, GPIO_LOW);
        break;
    }
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

    set_lamps(tl, TL_RED);

    return true;
}

void tl_update(traffic_light_t *tl)
{
    if (tl == NULL || tl->gpio == NULL || tl->clock == NULL) {
        return;
    }
    if (tl->gpio->read == NULL || tl->clock->now_ms == NULL) {
        return;
    }

    /* 1. Read the clock once */
    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    /* 2. Read the button; if pressed, remember the request */
    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    /* 3. Check phase elapsed duration (wrap-safe calculation) */
    uint32_t elapsed = now - tl->phase_start_ms;
    bool should_change = false;
    tl_phase_t next_phase = tl->phase;

    switch (tl->phase) {
    case TL_RED:
        if (elapsed >= TL_RED_MS) {
            should_change = true;
            next_phase = TL_RED_YELLOW;
        }
        break;

    case TL_RED_YELLOW:
        if (elapsed >= TL_RED_YELLOW_MS) {
            should_change = true;
            next_phase = TL_GREEN;
        }
        break;

    case TL_GREEN:
        if (elapsed >= TL_GREEN_MS) {
            should_change = true;
            next_phase = TL_YELLOW;
        } else if (tl->ped_request && (elapsed >= TL_GREEN_MIN_MS)) {
            should_change = true;
            next_phase = TL_YELLOW;
        }
        break;

    case TL_YELLOW:
        if (elapsed >= TL_YELLOW_MS) {
            should_change = true;
            next_phase = TL_RED;
        }
        break;
    }

    if (should_change) {
        tl->phase = next_phase;
        tl->phase_start_ms = now;

        /* Entering YELLOW clears the pedestrian request */
        if (next_phase == TL_YELLOW) {
            tl->ped_request = false;
        }

        set_lamps(tl, next_phase);
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