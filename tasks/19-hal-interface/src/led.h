/*
 * led.h - LED and button drivers written against the GPIO HAL.
 *
 * These drivers never touch hardware. They only call gpio->write() and
 * gpio->read() through the hal_gpio_t interface (see hal/hal_gpio.h). On the
 * PC the tests plug in a mock; on a board you plug in a real GPIO
 * implementation, and this code moves over unchanged.
 *
 * "Active low": on many boards an LED is wired so that driving the pin LOW
 * turns it on (and a button pulls the pin LOW when pressed). The driver hides
 * this, so the rest of the firmware only thinks "on/off" and "pressed".
 *
 * Do not change this file.
 */
#ifndef LED_H
#define LED_H

#include <stdbool.h>
#include <stdint.h>

#include "hal_gpio.h"

typedef struct {
    const hal_gpio_t *gpio;
    uint8_t pin;
    bool active_low;
    bool on; /* logical state, as last set by the driver */
} led_t;

/*
 * Remember the settings and switch the LED OFF (one write).
 * Returns false and does nothing if led or gpio is NULL or gpio->write is NULL.
 */
bool led_init(led_t *led, const hal_gpio_t *gpio, uint8_t pin, bool active_low);

/*
 * Switch the LED on or off: exactly one write to the pin.
 * active_low == false: on -> GPIO_HIGH, off -> GPIO_LOW
 * active_low == true:  on -> GPIO_LOW,  off -> GPIO_HIGH
 * NULL led: nothing.
 */
void led_set(led_t *led, bool on);

/* Invert the LED (one write). NULL: nothing. */
void led_toggle(led_t *led);

/* The logical state (from the driver's memory; does not read the pin). NULL -> false. */
bool led_is_on(const led_t *led);

typedef struct {
    const hal_gpio_t *gpio;
    uint8_t pin;
    bool active_low;
} button_t;

/* Returns false and does nothing if button or gpio is NULL or gpio->read is NULL. */
bool button_init(button_t *button, const hal_gpio_t *gpio, uint8_t pin, bool active_low);

/*
 * Read the pin (one read per call) and say whether the button is pressed.
 * active_low == false: GPIO_HIGH = pressed; active_low == true: GPIO_LOW = pressed.
 * NULL -> false.
 */
bool button_is_pressed(const button_t *button);

#endif /* LED_H */
