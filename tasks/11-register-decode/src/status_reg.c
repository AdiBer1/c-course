#include "status_reg.h"

#include <stddef.h>

/* Layout: defined once, used everywhere. */
#define READY_MASK      (1u << 0)
#define ERROR_MASK      (1u << 1)
#define OVERRUN_MASK    (1u << 2)

#define MODE_SHIFT      4u
#define MODE_MASK       0xFu
#define MODE_INVALID_FIELD 0xFu

#define FIFO_SHIFT      8u
#define FIFO_MASK       0xFFu

#define TEMP_SHIFT      16u
#define TEMP_MASK       0xFFFu
#define TEMP_SIGN_BIT   0x800u
#define TEMP_RANGE      4096 /* 2^12 */
#define TEMP_MIN        (-2048)
#define TEMP_MAX        2047

#define VERSION_SHIFT   28u
#define VERSION_MASK    0xFu

status_t status_decode(uint32_t reg)
{
    status_t s;
    uint32_t mode = (reg >> MODE_SHIFT) & MODE_MASK;
    uint32_t raw_temp = (reg >> TEMP_SHIFT) & TEMP_MASK;
    int32_t temp = (int32_t)raw_temp;

    if ((raw_temp & TEMP_SIGN_BIT) != 0u) {
        temp -= TEMP_RANGE; /* sign extension */
    }

    s.ready = (reg & READY_MASK) != 0u;
    s.error = (reg & ERROR_MASK) != 0u;
    s.overrun = (reg & OVERRUN_MASK) != 0u;
    s.mode = (mode <= 3u) ? (dev_mode_t)mode : DEV_MODE_INVALID;
    s.fifo_level = (uint8_t)((reg >> FIFO_SHIFT) & FIFO_MASK);
    s.temp_sixteenths = (int16_t)temp;
    s.version = (uint8_t)((reg >> VERSION_SHIFT) & VERSION_MASK);
    return s;
}

uint32_t status_encode(const status_t *s)
{
    uint32_t reg = 0u;
    uint32_t mode_field;
    int32_t temp;

    if (s == NULL) {
        return 0u;
    }

    switch (s->mode) {
    case DEV_MODE_IDLE:
    case DEV_MODE_MEASURE:
    case DEV_MODE_CALIBRATE:
    case DEV_MODE_SLEEP:
        mode_field = (uint32_t)s->mode;
        break;
    default:
        mode_field = MODE_INVALID_FIELD;
        break;
    }

    temp = s->temp_sixteenths;
    if (temp < TEMP_MIN) {
        temp = TEMP_MIN;
    }
    if (temp > TEMP_MAX) {
        temp = TEMP_MAX;
    }

    if (s->ready) {
        reg |= READY_MASK;
    }
    if (s->error) {
        reg |= ERROR_MASK;
    }
    if (s->overrun) {
        reg |= OVERRUN_MASK;
    }
    reg |= (mode_field & MODE_MASK) << MODE_SHIFT;
    reg |= ((uint32_t)s->fifo_level & FIFO_MASK) << FIFO_SHIFT;
    reg |= ((uint32_t)temp & TEMP_MASK) << TEMP_SHIFT;
    reg |= ((uint32_t)s->version & VERSION_MASK) << VERSION_SHIFT;
    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    uint32_t raw;

    if (reg == NULL) {
        return status_decode(0u);
    }
    raw = *reg; /* the only read of the register */
    return status_decode(raw);
}

int16_t status_temp_celsius(const status_t *s)
{
    int32_t t;

    if (s == NULL) {
        return 0;
    }
    t = s->temp_sixteenths;
    if (t >= 0) {
        return (int16_t)((t + 8) / 16);
    }
    return (int16_t)((t - 8) / 16);
}