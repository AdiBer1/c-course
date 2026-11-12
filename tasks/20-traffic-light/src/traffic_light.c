
#include <stddef.h>
#include "traffic_light.h"

/* Set output pins for red, yellow, and green lamps based on current phase */
static void apply_phase_lamps(const hal_gpio_t *gpio, tl_phase_t phase)
{
    gpio_level_t red = GPIO_LOW;
    gpio_level_t yellow = GPIO_LOW;
    gpio_level_t green = GPIO_LOW;

    switch (phase) {
    case TL_RED:
        red = GPIO_HIGH;
        break;
    case TL_RED_YELLOW:
        red = GPIO_HIGH;
        yellow = GPIO_HIGH;
        break;
    case TL_GREEN:
        green = GPIO_HIGH;
        break;
    case TL_YELLOW:
        yellow = GPIO_HIGH;
        break;
    default:
        red = GPIO_HIGH; /* Fail-safe to red on invalid state */
        break;
    }

    gpio->write(gpio->ctx, (uint8_t)TL_PIN_RED, red);
    gpio->write(gpio->ctx, (uint8_t)TL_PIN_YELLOW, yellow);
    gpio->write(gpio->ctx, (uint8_t)TL_PIN_GREEN, green);
}

/* Validate interfaces and initialize controller in RED phase */
bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock)
{
    if (tl == NULL || gpio == NULL || clock == NULL ||
        gpio->write == NULL || gpio->read == NULL || clock->now_ms == NULL) {
        return false;
    }

    tl->gpio = gpio;
    tl->clock = clock;
    tl->phase = TL_RED;
    tl->phase_start_ms = clock->now_ms(clock->ctx);
    tl->ped_request = false;

    apply_phase_lamps(gpio, TL_RED);
    return true;
}

/* Non-blocking step: poll button, check elapsed time, and advance at most one phase */
void tl_update(traffic_light_t *tl)
{
    if (tl == NULL) {
        return;
    }

    /* Read clock once per update */
    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    /* Latch pedestrian request when button is pressed */
    if (tl->gpio->read(tl->gpio->ctx, (uint8_t)TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    /* Wrap-safe elapsed time calculation */
    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);
    bool phase_done = false;
    tl_phase_t next_phase = tl->phase;

    switch (tl->phase) {
    case TL_RED:
        if (elapsed >= TL_RED_MS) {
            next_phase = TL_RED_YELLOW;
            phase_done = true;
        }
        break;
    case TL_RED_YELLOW:
        if (elapsed >= TL_RED_YELLOW_MS) {
            next_phase = TL_GREEN;
            phase_done = true;
        }
        break;
    case TL_GREEN:
        /* End early if pedestrian is waiting and minimum green time has passed */
        if (elapsed >= TL_GREEN_MS || (tl->ped_request && elapsed >= TL_GREEN_MIN_MS)) {
            next_phase = TL_YELLOW;
            phase_done = true;
        }
        break;
    case TL_YELLOW:
        if (elapsed >= TL_YELLOW_MS) {
            next_phase = TL_RED;
            phase_done = true;
        }
        break;
    default:
        next_phase = TL_RED;
        phase_done = true;
        break;
    }

    /* Apply transition and reset pedestrian request upon entering YELLOW */
    if (phase_done) {
        tl->phase = next_phase;
        tl->phase_start_ms = now;
        if (next_phase == TL_YELLOW) {
            tl->ped_request = false;
        }
        apply_phase_lamps(tl->gpio, next_phase);
    }
}

/* Return current traffic light phase (defaults to TL_RED if NULL) */
tl_phase_t tl_phase(const traffic_light_t *tl)
{
    return (tl == NULL) ? TL_RED : tl->phase;
}

/* Return true if a pedestrian button press is queued */
bool tl_pedestrian_waiting(const traffic_light_t *tl)
{
    return (tl == NULL) ? false : tl->ped_request;
}

