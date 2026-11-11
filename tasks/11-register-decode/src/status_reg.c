
#include "status_reg.h"

#include <stddef.h>

/* Register field positions and masks */
#define READY_BIT       0u
#define ERROR_BIT       1u
#define OVERRUN_BIT     2u

#define MODE_SHIFT      4u
#define FIFO_SHIFT      8u
#define TEMP_SHIFT      16u
#define VERSION_SHIFT   28u

#define MODE_MASK       0x0Fu
#define FIFO_MASK       0xFFu
#define TEMP_MASK       0x0FFFu
#define VERSION_MASK    0x0Fu

#define TEMP_SIGN_BIT   0x0800u
#define TEMP_RANGE      4096
#define TEMP_MAX        2047
#define TEMP_MIN       (-2048)

status_t status_decode(uint32_t reg)
{
    status_t s = {0};

    s.ready = ((reg >> READY_BIT) & 1u) != 0u;
    s.error = ((reg >> ERROR_BIT) & 1u) != 0u;
    s.overrun = ((reg >> OVERRUN_BIT) & 1u) != 0u;

    uint32_t mode = (reg >> MODE_SHIFT) & MODE_MASK;
    s.mode = (mode <= 3u) ? (dev_mode_t)mode : DEV_MODE_INVALID;

    s.fifo_level = (uint8_t)((reg >> FIFO_SHIFT) & FIFO_MASK);

    uint32_t raw_temp = (reg >> TEMP_SHIFT) & TEMP_MASK;

    if ((raw_temp & TEMP_SIGN_BIT) != 0u) {
        s.temp_sixteenths =
            (int16_t)((int32_t)raw_temp - TEMP_RANGE);
    } else {
        s.temp_sixteenths = (int16_t)raw_temp;
    }

    s.version = (uint8_t)((reg >> VERSION_SHIFT) & VERSION_MASK);

    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }

    uint32_t reg = 0u;

    if (s->ready) {
        reg |= (uint32_t)1u << READY_BIT;
    }

    if (s->error) {
        reg |= (uint32_t)1u << ERROR_BIT;
    }

    if (s->overrun) {
        reg |= (uint32_t)1u << OVERRUN_BIT;
    }

    uint32_t mode = (s->mode >= DEV_MODE_IDLE &&
                     s->mode <= DEV_MODE_SLEEP)
                        ? (uint32_t)s->mode
                        : MODE_MASK;

    reg |= mode << MODE_SHIFT;

    reg |= (uint32_t)s->fifo_level << FIFO_SHIFT;

    int32_t temp = s->temp_sixteenths;

    if (temp > TEMP_MAX) {
        temp = TEMP_MAX;
    } else if (temp < TEMP_MIN) {
        temp = TEMP_MIN;
    }

    reg |= ((uint32_t)temp & TEMP_MASK) << TEMP_SHIFT;

    reg |= ((uint32_t)s->version & VERSION_MASK) << VERSION_SHIFT;

    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL) {
        return status_decode(0u);
    }

    uint32_t value = *reg;

    return status_decode(value);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }

    int32_t temp = s->temp_sixteenths;

    if (temp >= 0) {
        return (int16_t)((temp + 8) / 16);
    }

    return (int16_t)(-((-temp + 8) / 16));
}
