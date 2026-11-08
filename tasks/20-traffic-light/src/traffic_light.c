#include "traffic_light.h"

#include <stddef.h>

static gpio_level_t level_of(bool on)
{
    return on ? GPIO_HIGH : GPIO_LOW;
}

static void set_lamps(const traffic_light_t *tl, bool red, bool yellow, bool green)
{
    const hal_gpio_t *g = tl->gpio;

    g->write(g->ctx, (uint8_t)TL_PIN_RED, level_of(red));
    g->write(g->ctx, (uint8_t)TL_PIN_YELLOW, level_of(yellow));
    g->write(g->ctx, (uint8_t)TL_PIN_GREEN, level_of(green));
}

static void show_phase(const traffic_light_t *tl)
{
    switch (tl->phase) {
    case TL_RED:
        set_lamps(tl, true, false, false);
        break;
    case TL_RED_YELLOW:
        set_lamps(tl, true, true, false);
        break;
    case TL_GREEN:
        set_lamps(tl, false, false, true);
        break;
    case TL_YELLOW:
        set_lamps(tl, false, true, false);
        break;
    default:
        set_lamps(tl, true, false, false); /* unknown: safest is red */
        break;
    }
}

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
        return 0u; /* unknown: end it at once */
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
    default:
        return TL_RED;
    }
}

static void enter_phase(traffic_light_t *tl, tl_phase_t phase, uint32_t now)
{
    tl->phase = phase;
    tl->phase_start_ms = now;
    if (phase == TL_YELLOW) {
        tl->ped_request = false;
    }
    show_phase(tl);
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
    tl->ped_request = false;
    enter_phase(tl, TL_RED, clock->now_ms(clock->ctx));
    return true;
}

void tl_update(traffic_light_t *tl)
{
    if (tl == NULL || tl->gpio == NULL || tl->clock == NULL) {
        return;
    }

    /* 1. read the clock once */
    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    /* 2. remember a button press */
    if (tl->gpio->read(tl->gpio->ctx, (uint8_t)TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    /* 3. is the current phase over? (wrap-safe) */
    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);
    bool over = elapsed >= phase_duration(tl->phase);

    if (tl->phase == TL_GREEN && tl->ped_request && elapsed >= TL_GREEN_MIN_MS) {
        over = true;
    }

    if (over) {
        enter_phase(tl, next_phase(tl->phase), now);
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
