#include "traffic_light.h"

#include <stddef.h>

static uint32_t phase_duration(tl_phase_t phase)
{
    switch (phase) {
    case TL_RED:
        return TL_RED_MS;
    case TL_RED_YELLOW:
        return TL_RED_YELLOW_MS;
    case TL_GREEN:
        return TL_GREEN_MS;
    case TL_YELLOW:
        return TL_YELLOW_MS;
    default:
        return TL_RED_MS;
    }
}

static tl_phase_t next_phase(tl_phase_t phase)
{
    switch (phase) {
    case TL_RED:
        return TL_RED_YELLOW;
    case TL_RED_YELLOW:
        return TL_GREEN;
    case TL_GREEN:
        return TL_YELLOW;
    case TL_YELLOW:
        return TL_RED;
    default:
        return TL_RED;
    }
}

static void set_lamps(const traffic_light_t *tl)
{
    bool red = (tl->phase == TL_RED) || (tl->phase == TL_RED_YELLOW);
    bool yellow = (tl->phase == TL_RED_YELLOW) || (tl->phase == TL_YELLOW);
    bool green = (tl->phase == TL_GREEN);

    tl->gpio->write(tl->gpio->ctx, TL_PIN_RED, red ? GPIO_HIGH : GPIO_LOW);
    tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, yellow ? GPIO_HIGH : GPIO_LOW);
    tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN, green ? GPIO_HIGH : GPIO_LOW);
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

    set_lamps(tl);

    return true;
}

void tl_update(traffic_light_t *tl)
{
    uint32_t now;
    uint32_t elapsed;
    bool phase_over;

    if (tl == NULL || tl->gpio == NULL || tl->clock == NULL) {
        return;
    }

    /* 1. read the clock once */
    now = tl->clock->now_ms(tl->clock->ctx);

    /* 2. remember a pedestrian request */
    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    /* 3. wrap-safe: unsigned subtraction gives the right elapsed time */
    elapsed = (uint32_t)(now - tl->phase_start_ms);

    phase_over = (elapsed >= phase_duration(tl->phase));

    if (tl->phase == TL_GREEN && tl->ped_request && elapsed >= TL_GREEN_MIN_MS) {
        phase_over = true;
    }

    if (!phase_over) {
        return;
    }

    tl->phase = next_phase(tl->phase);
    tl->phase_start_ms = now;
    set_lamps(tl);

    if (tl->phase == TL_YELLOW) {
        tl->ped_request = false;
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