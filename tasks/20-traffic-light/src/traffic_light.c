#include "traffic_light.h"

#include <stddef.h>

static void set_lamps(const traffic_light_t *tl, tl_phase_t phase)
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
    }

    tl->gpio->write(tl->gpio->ctx, TL_PIN_RED, red);
    tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, yellow);
    tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN, green);
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
    }
    return 0;
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
    }
    return TL_RED;
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
    if (tl == NULL || tl->gpio == NULL || tl->clock == NULL || tl->clock->now_ms == NULL) {
        return;
    }

    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);
    bool phase_over = elapsed >= phase_duration(tl->phase);
    bool pedestrian_cut = (tl->phase == TL_GREEN) && tl->ped_request
                          && (elapsed >= TL_GREEN_MIN_MS);

    if (!phase_over && !pedestrian_cut) {
        return;
    }

    tl->phase = next_phase(tl->phase);
    tl->phase_start_ms = now;
    if (tl->phase == TL_YELLOW) {
        tl->ped_request = false;
    }
    set_lamps(tl, tl->phase);
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
    return tl != NULL && tl->ped_request;
}