#include "traffic_light.h"

#include <stddef.h>

static void set_lamps(const traffic_light_t *tl, tl_phase_t phase)
{
    tl->gpio->write(tl->gpio->ctx, TL_PIN_RED,
                    (phase == TL_RED || phase == TL_RED_YELLOW) ? GPIO_HIGH : GPIO_LOW);
    tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW,
                    (phase == TL_RED_YELLOW || phase == TL_YELLOW) ? GPIO_HIGH : GPIO_LOW);
    tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN,
                    (phase == TL_GREEN) ? GPIO_HIGH : GPIO_LOW);
}

bool tl_init(traffic_light_t *tl, const hal_gpio_t *gpio, const hal_clock_t *clock)
{
    if (!tl || !gpio || !clock) return false;
    if (!gpio->write || !gpio->read || !clock->now_ms) return false;
    tl->gpio          = gpio;
    tl->clock         = clock;
    tl->phase         = TL_RED;
    tl->phase_start_ms = clock->now_ms(clock->ctx);
    tl->ped_request   = false;
    set_lamps(tl, TL_RED);
    return true;
}

void tl_update(traffic_light_t *tl)
{
    if (!tl) return;

    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH)
        tl->ped_request = true;

    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);
    bool phase_over  = false;

    switch (tl->phase) {
        case TL_RED:
            phase_over = elapsed >= TL_RED_MS;
            break;
        case TL_RED_YELLOW:
            phase_over = elapsed >= TL_RED_YELLOW_MS;
            break;
        case TL_GREEN:
            phase_over = elapsed >= TL_GREEN_MS ||
                         (tl->ped_request && elapsed >= TL_GREEN_MIN_MS);
            break;
        case TL_YELLOW:
            phase_over = elapsed >= TL_YELLOW_MS;
            break;
    }

    if (!phase_over) return;

    tl_phase_t next;
    switch (tl->phase) {
        case TL_RED:        next = TL_RED_YELLOW; break;
        case TL_RED_YELLOW: next = TL_GREEN;      break;
        case TL_GREEN:      next = TL_YELLOW;     break;
        case TL_YELLOW:     next = TL_RED;        break;
        default:            next = TL_RED;        break;
    }

    if (next == TL_YELLOW)
        tl->ped_request = false;

    tl->phase          = next;
    tl->phase_start_ms = now;
    set_lamps(tl, next);
}

tl_phase_t tl_phase(const traffic_light_t *tl)
{
    if (!tl) return TL_RED;
    return tl->phase;
}

bool tl_pedestrian_waiting(const traffic_light_t *tl)
{
    if (!tl) return false;
    return tl->ped_request;
}
