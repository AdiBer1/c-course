#include "traffic_light.h"

#include <stddef.h>

static void write_lamp(const traffic_light_t *tl, uint8_t pin, bool on)
{
    gpio_level_t x = on ? GPIO_HIGH : GPIO_LOW;
    tl->gpio->write(tl->gpio->ctx, pin, x);
}

static void set_lamps(const traffic_light_t *tl)
{
    bool red_on = (tl->phase == TL_RED) || (tl->phase == TL_RED_YELLOW);
    bool green_on = (tl->phase == TL_GREEN);
    bool yellow_on = (tl->phase == TL_YELLOW) || (tl->phase == TL_RED_YELLOW);
    write_lamp(tl, TL_PIN_RED, red_on);
    write_lamp(tl, TL_PIN_YELLOW, yellow_on);
    write_lamp(tl, TL_PIN_GREEN, green_on);
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
    if (tl == NULL) {
        return;
    }
    uint32_t now = tl->clock->now_ms(tl->clock->ctx);
    tl->ped_request = tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH || tl->ped_request;
    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);
    bool over = false;
    tl_phase_t next = tl->phase;

    switch (tl->phase) {
    case TL_RED:
        if (elapsed >= TL_RED_MS) {
            next = TL_RED_YELLOW;
            over = true;
        }
        break;
    case TL_RED_YELLOW:
        if (elapsed >= TL_RED_YELLOW_MS) {
            next = TL_GREEN;
            over = true;
        }
        break;
    case TL_GREEN:
        if (elapsed >= TL_GREEN_MS || (tl->ped_request && elapsed >= TL_GREEN_MIN_MS)) {
            next = TL_YELLOW;
            over = true;
        }
        break;
    case TL_YELLOW:
        if (elapsed >= TL_YELLOW_MS) {
            next = TL_RED;
            over = true;
        }
        break;
    default:
        return;
    }
    if (over) {
        tl->phase = next;
        tl->phase_start_ms = now;
        set_lamps(tl);
        if (next == TL_YELLOW) {
            tl->ped_request = false;
        }
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
