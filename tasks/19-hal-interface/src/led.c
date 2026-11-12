#include "led.h"

#include <stddef.h>

static gpio_level_t logical_to_level(bool on, bool active_low)
{
    bool drive_high = active_low ? !on : on;
    return drive_high ? GPIO_HIGH : GPIO_LOW;
}

bool led_init(led_t *led, const hal_gpio_t *gpio, uint8_t pin, bool active_low)
{
    if (led == NULL || gpio == NULL || gpio->write == NULL) {
        return false;
    }
    led->gpio = gpio;
    led->pin = pin;
    led->active_low = active_low;
    led->on = false;
    led->gpio->write(led->gpio->ctx, led->pin, logical_to_level(false, active_low));
    return true;
}

void led_set(led_t *led, bool on)
{
    if (led == NULL || led->gpio == NULL || led->gpio->write == NULL) {
        return;
    }
    led->on = on;
    led->gpio->write(led->gpio->ctx, led->pin, logical_to_level(on, led->active_low));
}

void led_toggle(led_t *led)
{
    if (led == NULL) {
        return;
    }
    led_set(led, !led->on);
}

bool led_is_on(const led_t *led)
{
    if (led == NULL) {
        return false;
    }
    return led->on;
}

bool button_init(button_t *button, const hal_gpio_t *gpio, uint8_t pin, bool active_low)
{
    if (button == NULL || gpio == NULL || gpio->read == NULL) {
        return false;
    }
    button->gpio = gpio;
    button->pin = pin;
    button->active_low = active_low;
    return true;
}

bool button_is_pressed(const button_t *button)
{
    if (button == NULL || button->gpio == NULL || button->gpio->read == NULL) {
        return false;
    }
    gpio_level_t level = button->gpio->read(button->gpio->ctx, button->pin);
    if (button->active_low) {
        return level == GPIO_LOW;
    }
    return level == GPIO_HIGH;
}