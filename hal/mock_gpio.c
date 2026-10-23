#include "mock_gpio.h"

#include <stddef.h>

static void mock_write(void *ctx, uint8_t pin, gpio_level_t level)
{
    mock_gpio_t *mock = ctx;
    if (pin >= MOCK_GPIO_PINS) {
        mock->bad_pin_count++;
        return;
    }
    mock->outputs[pin] = level;
    mock->write_count++;
    mock->last_pin = pin;
}

static gpio_level_t mock_read(void *ctx, uint8_t pin)
{
    mock_gpio_t *mock = ctx;
    if (pin >= MOCK_GPIO_PINS) {
        mock->bad_pin_count++;
        return GPIO_LOW;
    }
    mock->read_count++;
    return mock->inputs[pin];
}

void mock_gpio_init(mock_gpio_t *mock, hal_gpio_t *hal)
{
    for (size_t i = 0; i < MOCK_GPIO_PINS; i++) {
        mock->outputs[i] = GPIO_LOW;
        mock->inputs[i] = GPIO_LOW;
    }
    mock->write_count = 0;
    mock->read_count = 0;
    mock->bad_pin_count = 0;
    mock->last_pin = 0;

    hal->write = mock_write;
    hal->read = mock_read;
    hal->ctx = mock;
}

void mock_gpio_set_input(mock_gpio_t *mock, uint8_t pin, gpio_level_t level)
{
    if (pin < MOCK_GPIO_PINS) {
        mock->inputs[pin] = level;
    }
}

gpio_level_t mock_gpio_output(const mock_gpio_t *mock, uint8_t pin)
{
    return pin < MOCK_GPIO_PINS ? mock->outputs[pin] : GPIO_LOW;
}
