#include "led.h"

#include <stddef.h>

/*
 * The ONLY way this driver reaches the hardware: gpio->write / gpio->read,
 * always passing gpio->ctx so the implementation finds its own data.
 */

/* Translate the logical state into the pin level, hiding active-low wiring. */
static gpio_level_t level_for(bool on, bool active_low)
{
    /* on XOR active_low: active-high ON -> HIGH, active-low ON -> LOW */
    return (on != active_low) ? GPIO_HIGH : GPIO_LOW;
}

/* One write to the pin, and remember the new logical state. */
static void apply(led_t *led, bool on)
{
    led->gpio->write(led->gpio->ctx, led->pin, level_for(on, led->active_low));
    led->on = on;
}

bool led_init(led_t *led, const hal_gpio_t *gpio, uint8_t pin, bool active_low)
{
    if (led == NULL || gpio == NULL || gpio->write == NULL) {
        return false;
    }
    led->gpio = gpio;
    led->pin = pin;
    led->active_low = active_low;
    apply(led, false); /* start OFF: exactly one write */
    return true;
}

void led_set(led_t *led, bool on)
{
    if (led == NULL || led->gpio == NULL || led->gpio->write == NULL) {
        return;
    }
    apply(led, on);
}

void led_toggle(led_t *led)
{
    if (led == NULL || led->gpio == NULL || led->gpio->write == NULL) {
        return;
    }
    apply(led, !led->on);
}

bool led_is_on(const led_t *led)
{
    /* From the driver's memory: no read of the pin. */
    return (led != NULL) && led->on;
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
    /* Exactly one read per call. */
    gpio_level_t level = button->gpio->read(button->gpio->ctx, button->pin);
    bool high = (level == GPIO_HIGH);
    /* active-high: HIGH = pressed; active-low: LOW = pressed */
    return high != button->active_low;
}