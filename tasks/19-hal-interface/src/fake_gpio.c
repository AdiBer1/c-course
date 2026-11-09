#include "fake_gpio.h"

#include <string.h>

static void fake_write(void *ctx, uint8_t pin, gpio_level_t level)
{
    fake_gpio_t *f = (fake_gpio_t *)ctx;
    if (f->log_count < FAKE_GPIO_LOG_SIZE) {
        f->log[f->log_count].pin   = pin;
        f->log[f->log_count].level = level;
        f->log_count++;
    }
    f->total_writes++;
}

static gpio_level_t fake_read(void *ctx, uint8_t pin)
{
    fake_gpio_t *f = (fake_gpio_t *)ctx;
    f->reads++;
    if (pin >= FAKE_GPIO_PINS) return GPIO_LOW;
    return f->input_level[pin];
}

void fake_gpio_init(fake_gpio_t *fake, hal_gpio_t *hal)
{
    if (!fake || !hal) return;
    memset(fake, 0, sizeof(*fake));
    hal->write = fake_write;
    hal->read  = fake_read;
    hal->ctx   = fake;
}
