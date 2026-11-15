#include "led.h"

#include <stddef.h>

/* Translate a logical on/off state into the electrical level for this LED. */
static gpio_level_t led_level_for(bool on, bool active_low)
{
    /* active high: on -> HIGH; active low: on -> LOW */
    return (on != active_low) ? GPIO_HIGH : GPIO_LOW;
}

bool led_init(led_t *led, const hal_gpio_t *gpio, uint8_t pin, bool active_low)
{
    if (led == NULL || gpio == NULL || gpio->write == NULL) {
        return false;
    }

    led->gpio = gpio;
    led->pin = pin;
    led->active_low = active_low;

    led_set(led, false); /* the single write: LED starts OFF */
    return true;
}

void led_set(led_t *led, bool on)
{
    if (led == NULL || led->gpio == NULL || led->gpio->write == NULL) {
        return;
    }

    led->gpio->write(led->gpio->ctx, led->pin, led_level_for(on, led->active_low));
    led->on = on;
}

void led_toggle(led_t *led)
{
    if (led == NULL) {
        return;
    }
    led_set(led, !led->on); /* exactly one write */
}

bool led_is_on(const led_t *led)
{
    if (led == NULL) {
        return false;
    }
    return led->on; /* driver memory only, never reads the pin */
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
