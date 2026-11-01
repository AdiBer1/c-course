#include "status_reg.h"

#include <stddef.h>

/* Field positions (lowest bit) and masks (already shifted down to bit 0). */
#define READY_POS    0u
#define ERROR_POS    1u
#define OVERRUN_POS  2u
#define MODE_POS     4u
#define MODE_MASK    0xFu
#define FIFO_POS     8u
#define FIFO_MASK    0xFFu
#define TEMP_POS     16u
#define TEMP_MASK    0xFFFu
#define TEMP_SIGN    0x800
#define TEMP_RANGE   0x1000
#define VERSION_POS  28u
#define VERSION_MASK 0xFu

#define TEMP_MIN (-2048)
#define TEMP_MAX 2047

status_t status_decode(uint32_t reg)
{
    status_t s = {0};
    uint32_t mode = (reg >> MODE_POS) & MODE_MASK;
    int32_t temp = (int32_t)((reg >> TEMP_POS) & TEMP_MASK);

    s.ready = ((reg >> READY_POS) & 1u) != 0u;
    s.error = ((reg >> ERROR_POS) & 1u) != 0u;
    s.overrun = ((reg >> OVERRUN_POS) & 1u) != 0u;

    s.mode = (mode <= 3u) ? (dev_mode_t)mode : DEV_MODE_INVALID;

    s.fifo_level = (uint8_t)((reg >> FIFO_POS) & FIFO_MASK);

    /* 12-bit two's complement -> sign extend */
    if ((temp & TEMP_SIGN) != 0) {
        temp -= TEMP_RANGE;
    }
    s.temp_sixteenths = (int16_t)temp;

    s.version = (uint8_t)((reg >> VERSION_POS) & VERSION_MASK);
    return s;
}

uint32_t status_encode(const status_t *s)
{
    uint32_t reg = 0u;
    uint32_t mode_bits;
    int32_t temp;

    if (s == NULL) {
        return 0u;
    }

    if (s->ready) {
        reg |= 1u << READY_POS;
    }
    if (s->error) {
        reg |= 1u << ERROR_POS;
    }
    if (s->overrun) {
        reg |= 1u << OVERRUN_POS;
    }

    switch (s->mode) {
    case DEV_MODE_IDLE:
    case DEV_MODE_MEASURE:
    case DEV_MODE_CALIBRATE:
    case DEV_MODE_SLEEP:
        mode_bits = (uint32_t)s->mode;
        break;
    default:
        mode_bits = MODE_MASK; /* 0xF = invalid */
        break;
    }
    reg |= mode_bits << MODE_POS;

    reg |= (uint32_t)s->fifo_level << FIFO_POS;

    temp = s->temp_sixteenths;
    if (temp < TEMP_MIN) {
        temp = TEMP_MIN;
    } else if (temp > TEMP_MAX) {
        temp = TEMP_MAX;
    }
    reg |= ((uint32_t)temp & TEMP_MASK) << TEMP_POS;

    reg |= ((uint32_t)s->version & VERSION_MASK) << VERSION_POS;

    return reg; /* reserved bit 3 stays 0 */
}

status_t status_read(const volatile uint32_t *reg)
{
    uint32_t value;

    if (reg == NULL) {
        return status_decode(0u);
    }
    value = *reg; /* exactly one read of the hardware */
    return status_decode(value);
}

int16_t status_temp_celsius(const status_t *s)
{
    int32_t t;
    int32_t c;

    if (s == NULL) {
        return 0;
    }
    t = s->temp_sixteenths;
    if (t >= 0) {
        c = (t + 8) / 16;
    } else {
        c = -((-t + 8) / 16);
    }
    return (int16_t)c;
}