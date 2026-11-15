#include "fake_gpio.h"

#include <string.h>

/* Same signature as hal_gpio_t.write */
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
    fake->total_writes++; /* always counted, even when the log is full */
}

/* Same signature as hal_gpio_t.read */
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

    /* empty log, counters 0, all inputs GPIO_LOW (== 0) */
    memset(fake, 0, sizeof(*fake));

    hal->write = fake_write;
    hal->read = fake_read;
    hal->ctx = fake;
}
