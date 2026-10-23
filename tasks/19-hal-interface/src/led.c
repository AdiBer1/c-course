#include "led.h"

#include <stddef.h>

/* TODO: implement the drivers using ONLY led->gpio->write / button->gpio->read. */

bool led_init(led_t *led, const hal_gpio_t *gpio, uint8_t pin, bool active_low)
{
    (void)led;
    (void)gpio;
    (void)pin;
    (void)active_low;
    return false;
}

void led_set(led_t *led, bool on)
{
    (void)led;
    (void)on;
}

void led_toggle(led_t *led)
{
    (void)led;
}

bool led_is_on(const led_t *led)
{
    (void)led;
    return false;
}

bool button_init(button_t *button, const hal_gpio_t *gpio, uint8_t pin, bool active_low)
{
    (void)button;
    (void)gpio;
    (void)pin;
    (void)active_low;
    return false;
}

bool button_is_pressed(const button_t *button)
{
    (void)button;
    return false;
}
