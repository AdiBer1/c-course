#include "traffic_light.h"

#include <stddef.h>

/* TODO: implement. Never wait or loop until time has passed! */
static void set_lamps(const traffic_light_t *tl, tl_phase_t phase)
{
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

    tl->gpio           = gpio;
    tl->clock          = clock;
    tl->phase          = TL_RED;
    tl->phase_start_ms = clock->now_ms(clock->ctx);
    tl->ped_request    = false;

    set_lamps(tl, TL_RED);

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

    /* 1. Read clock once */
    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    /* 2. Read button; if pressed, remember request */
    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    /* 3. Check phase duration and transition if over */
    uint32_t elapsed      = (uint32_t)(now - tl->phase_start_ms);
    bool phase_over       = false;
    tl_phase_t next_phase = tl->phase;

    switch (tl->phase) {
    case TL_RED:
        if (elapsed >= TL_RED_MS) {
            phase_over = true;
            next_phase = TL_RED_YELLOW;
        }
        break;

    case TL_RED_YELLOW:
        if (elapsed >= TL_RED_YELLOW_MS) {
            phase_over = true;
            next_phase = TL_GREEN;
        }
        break;

    case TL_GREEN:
        if (elapsed >= TL_GREEN_MS || (tl->ped_request && elapsed >= TL_GREEN_MIN_MS)) {
            phase_over = true;
            next_phase = TL_YELLOW;
        }
        break;

    case TL_YELLOW:
        if (elapsed >= TL_YELLOW_MS) {
            phase_over = true;
            next_phase = TL_RED;
        }
        break;
    }

    if (phase_over) {
        tl->phase          = next_phase;
        tl->phase_start_ms = now;
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
