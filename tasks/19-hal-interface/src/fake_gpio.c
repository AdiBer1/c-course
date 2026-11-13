#include "fake_gpio.h"

/*
 * TODO:
 *   1. write a static function with the same signature as hal_gpio_t's write
 *      pointer: void fake_write(void *ctx, uint8_t pin, gpio_level_t level)
 *      (ctx is the fake_gpio_t you stored in hal->ctx)
 *   2. the same for read
 *   3. in fake_gpio_init: clear the fake, then point hal->write, hal->read
 *     #include "fake_gpio.h"

/*
 * The other side of the interface. Both functions have exactly the types of
 * the hal_gpio_t function pointers. All state lives in the fake_gpio_t that
 * fake_gpio_init stored in hal->ctx, so several fakes can exist at once.
 */

static void fake_write(void *ctx, uint8_t pin, gpio_level_t level)
{
    fake_gpio_t *fake = (fake_gpio_t *)ctx;

    if (fake == NULL) {
        return;
    }
    if (fake->log_count < FAKE_GPIO_LOG_SIZE) {
        fake->log[fake->log_count].pin = pin;
        fake->log[fake->log_count].level = level;
        fake->log_count++;
    }
    fake->total_writes++; /* always counted, also when the log is full */
}

static gpio_level_t fake_read(void *ctx, uint8_t pin)
{
    fake_gpio_t *fake = (fake_gpio_t *)ctx;

    if (fake == NULL) {
        return GPIO_LOW;
    }
    fake->reads++;
    if (pin >= FAKE_GPIO_PINS) {
        return GPIO_LOW; /* never index outside input_level[] */
    }
    return fake->input_level[pin];
}

void fake_gpio_init(fake_gpio_t *fake, hal_gpio_t *hal)
{
    if (fake == NULL || hal == NULL) {
        return;
    }

    /* Clear the fake: empty log, counters 0, all inputs LOW. */
    for (size_t i = 0u; i < FAKE_GPIO_LOG_SIZE; i++) {
        fake->log[i].pin = 0u;
        fake->log[i].level = GPIO_LOW;
    }
    fake->log_count = 0u;
    fake->total_writes = 0u;
    for (size_t i = 0u; i < FAKE_GPIO_PINS; i++) {
        fake->input_level[i] = GPIO_LOW;
    }
    fake->reads = 0u;

    /* Plug the fake into the interface: function ADDRESSES plus its data. */
    hal->write = fake_write;
    hal->read = fake_read;
    hal->ctx = fake;
} and hal->ctx at them
 */

void fake_gpio_init(fake_gpio_t *fake, hal_gpio_t *hal)
{
    (void)fake;
    (void)hal;
}
