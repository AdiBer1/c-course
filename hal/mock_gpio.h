/*
 * mock_gpio.h - a fake GPIO port for tests (implements hal_gpio_t).
 *
 * Outputs written by the driver are stored so a test can check them; inputs
 * are set by the test to simulate buttons. Nothing here touches hardware.
 */
#ifndef MOCK_GPIO_H
#define MOCK_GPIO_H

#include <stdint.h>

#include "hal_gpio.h"

#define MOCK_GPIO_PINS 16u

typedef struct {
    gpio_level_t outputs[MOCK_GPIO_PINS];
    gpio_level_t inputs[MOCK_GPIO_PINS];
    uint32_t write_count;   /* number of write() calls with a valid pin */
    uint32_t read_count;    /* number of read() calls with a valid pin */
    uint32_t bad_pin_count; /* calls with pin >= MOCK_GPIO_PINS (ignored) */
    uint8_t last_pin;       /* pin of the last valid write() */
} mock_gpio_t;

/* Reset the mock (all pins LOW, counters 0) and point `hal` at it. */
void mock_gpio_init(mock_gpio_t *mock, hal_gpio_t *hal);

/* Test side: simulate the outside world driving input pin `pin`. */
void mock_gpio_set_input(mock_gpio_t *mock, uint8_t pin, gpio_level_t level);

/* Test side: what did the driver write to output pin `pin` last? */
gpio_level_t mock_gpio_output(const mock_gpio_t *mock, uint8_t pin);

#endif /* MOCK_GPIO_H */
