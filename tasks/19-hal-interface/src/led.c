#include "led.h"

#include <stddef.h>

/* Pin level that corresponds to the logical LED state. */
static gpio_level_t led_level(const led_t *led, bool on)
{
    bool drive_high = led->active_low ? !on : on;

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

    /* start with the LED off: exactly one write */
    gpio->write(gpio->ctx, pin, led_level(led, false));
    return true;
}

void led_set(led_t *led, bool on)
{
    if (led == NULL || led->gpio == NULL || led->gpio->write == NULL) {
        return;
    }
    led->on = on;
    led->gpio->write(led->gpio->ctx, led->pin, led_level(led, on));
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
    return led->on; /* from the driver's memory, the pin is not read */
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
    gpio_level_t level;

    if (button == NULL || button->gpio == NULL || button->gpio->read == NULL) {
        return false;
    }
    /* exactly one read per call */
    level = button->gpio->read(button->gpio->ctx, button->pin);

    if (button->active_low) {
        return level == GPIO_LOW;
    }
    return level == GPIO_HIGH;
}