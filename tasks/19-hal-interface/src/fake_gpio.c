#include "fake_gpio.h"

/*
 * TODO:
 *   1. write a static function with the same signature as hal_gpio_t's write
 *      pointer: void fake_write(void *ctx, uint8_t pin, gpio_level_t level)
 *      (ctx is the fake_gpio_t you stored in hal->ctx)
 *   2. the same for read
 *   3. in fake_gpio_init: clear the fake, then point hal->write, hal->read
 *      and hal->ctx at them
 */

#include "fake_gpio.h"
#include <string.h>

static void fake_write(void *ctx, uint8_t pin, gpio_level_t level)
{
    fake_gpio_t *fake = (fake_gpio_t *)ctx;
    if (fake == NULL) {
        return;
    }

    fake->total_writes++;

    if (fake->log_count < FAKE_GPIO_LOG_SIZE) {
        fake->log[fake->log_count].pin = pin;
        fake->log[fake->log_count].level = level;
        fake->log_count++;
    }
}

static gpio_level_t fake_read(void *ctx, uint8_t pin)
{
    fake_gpio_t *fake = (fake_gpio_t *)ctx;
    if (fake == NULL) {
        return GPIO_LOW;
    }

    fake->reads++;

    if (pin >= FAKE_GPIO_PINS) {
        return GPIO_LOW;
    }

    return fake->input_level[pin];
}

void fake_gpio_init(fake_gpio_t *fake, hal_gpio_t *hal)
{
    if (fake == NULL || hal == NULL) {
        return;
    }

    memset(fake, 0, sizeof(fake_gpio_t));

    hal->write = fake_write;
    hal->read = fake_read;
    hal->ctx = fake;
}