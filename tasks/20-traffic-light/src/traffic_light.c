#include "traffic_light.h"

#include <stddef.h>

/*
 * NON-BLOCKING: every function here does a little work and returns.
 * There is no loop that waits for time to pass; tl_update() is simply
 * called again and again by the main loop.
 */

/* Drive one lamp through the HAL. */
static void lamp(const traffic_light_t *tl, uint8_t pin, bool on)
{
    tl->gpio->write(tl->gpio->ctx, pin, on ? GPIO_HIGH : GPIO_LOW);
}

/* Set all three lamps for a phase. */
static void set_lamps(const traffic_light_t *tl, tl_phase_t phase)
{
    lamp(tl, TL_PIN_RED, phase == TL_RED || phase == TL_RED_YELLOW);
    lamp(tl, TL_PIN_YELLOW, phase == TL_RED_YELLOW || phase == TL_YELLOW);
    lamp(tl, TL_PIN_GREEN, phase == TL_GREEN);
}

/* Switch to a new phase that starts at `now`. */
static void enter(traffic_light_t *tl, tl_phase_t phase, uint32_t now)
{
    tl->phase = phase;
    tl->phase_start_ms = now;
    if (phase == TL_YELLOW) {
        tl->ped_request = false; /* green is over: the request is served */
    }
    set_lamps(tl, phase);
}

bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock)
{
    if (tl == NULL || gpio == NULL || clock == NULL || gpio->write == NULL ||
        gpio->read == NULL || clock->now_ms == NULL) {
        return false;
    }
    tl->gpio = gpio;
    tl->clock = clock;
    tl->ped_request = false;
    enter(tl, TL_RED, clock->now_ms(clock->ctx));
    return true;
}

void tl_update(traffic_light_t *tl)
{
    if (tl == NULL || tl->gpio == NULL || tl->clock == NULL) {
        return;
    }

    /* 1. Read the clock ONCE. */
    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    /*
     * Wrap-safe elapsed time: unsigned subtraction is computed modulo 2^32,
     * so it is correct even when the counter wrapped from 0xFFFFFFFF to 0.
     * (Never compare now >= start + duration: that sum can wrap.)
     */
    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);

    /* 2. Remember a pedestrian request. */
    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    /* 3. At most ONE phase change; the new phase starts "now". */
    switch (tl->phase) {
    case TL_RED:
        if (elapsed >= TL_RED_MS) {
            enter(tl, TL_RED_YELLOW, now);
        }
        break;
    case TL_RED_YELLOW:
        if (elapsed >= TL_RED_YELLOW_MS) {
            enter(tl, TL_GREEN, now);
        }
        break;
    case TL_GREEN:
        if (elapsed >= TL_GREEN_MS || (tl->ped_request && elapsed >= TL_GREEN_MIN_MS)) {
            enter(tl, TL_YELLOW, now);
        }
        break;
    case TL_YELLOW:
        if (elapsed >= TL_YELLOW_MS) {
            enter(tl, TL_RED, now);
        }
        break;
    default:
        /* Corrupted phase value: fall back to the safe state. */
        enter(tl, TL_RED, now);
        break;
    }
}

tl_phase_t tl_phase(const traffic_light_t *tl)
{
    return (tl == NULL) ? TL_RED : tl->phase;
}

bool tl_pedestrian_waiting(const traffic_light_t *tl)
{
    return (tl != NULL) && tl->ped_request;
}