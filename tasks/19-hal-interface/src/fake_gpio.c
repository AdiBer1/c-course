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

void fake_gpio_init(fake_gpio_t *fake, hal_gpio_t *hal)
{
    (void)fake;
    (void)hal;
}
