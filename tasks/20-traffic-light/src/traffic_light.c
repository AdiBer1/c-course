#include "traffic_light.h"

#include <stddef.h>

static void set_lamps(const traffic_light_t *tl)
{
    switch (tl->phase) {
    case TL_RED:
        tl->gpio->write(tl->gpio->ctx, TL_PIN_RED,    GPIO_HIGH);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, GPIO_LOW );
        tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN,  GPIO_LOW);
        break;
    case TL_RED_YELLOW:
        tl->gpio->write(tl->gpio->ctx, TL_PIN_RED,    GPIO_HIGH);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, GPIO_HIGH );
        tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN,  GPIO_LOW);
        break;
    case TL_GREEN:
        tl->gpio->write(tl->gpio->ctx, TL_PIN_RED,    GPIO_LOW);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, GPIO_LOW );
        tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN,  GPIO_HIGH);
        break;
    case TL_YELLOW:
        tl->gpio->write(tl->gpio->ctx, TL_PIN_RED,    GPIO_LOW);
        tl->gpio->write(tl->gpio->ctx, TL_PIN_YELLOW, GPIO_HIGH );
        tl->gpio->write(tl->gpio->ctx, TL_PIN_GREEN,  GPIO_LOW);
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
    set_lamps(tl);
    return true;
}

void tl_update(traffic_light_t *tl)
{
    if (tl == NULL) {
        return;
    }

    uint32_t now = tl->clock->now_ms(tl->clock->ctx);

    if (tl->gpio->read(tl->gpio->ctx, TL_PIN_BUTTON) == GPIO_HIGH) {
        tl->ped_request = true;
    }

    uint32_t elapsed = (uint32_t)(now - tl->phase_start_ms);

    /* 4. האם השלב נגמר, ומה השלב הבא */
    switch (tl->phase) {
    case TL_RED:
        if (elapsed >= TL_RED_MS) {
            tl->phase = TL_RED_YELLOW;
            tl->phase_start_ms = now;
            set_lamps(tl);
        }
        break;
    case TL_RED_YELLOW:
        if (elapsed >= TL_RED_YELLOW_MS) {
            tl->phase = TL_GREEN;
            tl->phase_start_ms = now;
            set_lamps(tl);
        }
        break;
    case TL_GREEN:
        if (elapsed >= TL_GREEN_MS || (tl->ped_request && elapsed >= TL_GREEN_MIN_MS)) {
            tl->phase = TL_YELLOW;
            tl->ped_request = false;      /* כניסה ל-YELLOW מנקה בקשה */
            tl->phase_start_ms = now;
            set_lamps(tl);
        }
        break;
    case TL_YELLOW:
        if (elapsed >= TL_YELLOW_MS) {
            tl->phase = TL_RED;
            tl->phase_start_ms = now;
            set_lamps(tl);
        }
        break;
    }
}

tl_phase_t tl_phase(const traffic_light_t *tl)
{
    if (tl == NULL) { return TL_RED; }
    return tl->phase;
}

bool tl_pedestrian_waiting(const traffic_light_t *tl)
{
    if (tl == NULL) { return false; }
    return tl->ped_request;
}
