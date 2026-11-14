#include "led.h"

#include <stddef.h>

/* TODO: implement the drivers using ONLY led->gpio->write / button->gpio->read. */

bool led_init(led_t *led, const hal_gpio_t *gpio, uint8_t pin, bool active_low)
{
    if (led == NULL || gpio == NULL || gpio->write == NULL) {
        return false;
    }
    led->gpio = gpio;
    led->pin = pin;
    led->active_low = active_low;
    led->on = false;
    gpio->write(gpio->ctx, pin, active_low ? GPIO_HIGH : GPIO_LOW);
    return true;
}

void led_set(led_t *led, bool on)
{
    if (led == NULL) {
        return;
    }

    led->on = on;

    gpio_level_t level = (on != led->active_low) ? GPIO_HIGH : GPIO_LOW;

    led->gpio->write(led->gpio->ctx, led->pin, level);
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
    if (button == NULL || button->gpio == NULL ||
        button->gpio->read == NULL) {
        return false;
    }

    gpio_level_t level = button->gpio->read(button->gpio->ctx, button->pin);

    return button->active_low ? (level == GPIO_LOW) : (level == GPIO_HIGH);
}
