#include "led.h"
#include <stddef.h>

bool led_init(led_t *led, const hal_gpio_t *gpio, uint8_t pin, bool active_low)
{
    if (led == NULL || gpio == NULL || gpio->write == NULL) {
        return false;
    }

    led->gpio = gpio;
    led->pin = pin;
    led->active_low = active_low;
    led->on = false;

    /* Turn OFF initial state */
    gpio_level_t physical_state = active_low ? GPIO_HIGH : GPIO_LOW;
    gpio->write(gpio->ctx, pin, physical_state);

    return true;
}

void led_set(led_t *led, bool on)
{
    if (led == NULL || led->gpio == NULL || led->gpio->write == NULL) {
        return;
    }

    led->on = on;
    gpio_level_t physical_state;

    if (led->active_low) {
        physical_state = on ? GPIO_LOW : GPIO_HIGH;
    } else {
        physical_state = on ? GPIO_HIGH : GPIO_LOW;
    }

    led->gpio->write(led->gpio->ctx, led->pin, physical_state);
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

    gpio_level_t physical_state = button->gpio->read(button->gpio->ctx, button->pin);

    if (button->active_low) {
        return (physical_state == GPIO_LOW);
    } else {
        return (physical_state == GPIO_HIGH);
    }
}