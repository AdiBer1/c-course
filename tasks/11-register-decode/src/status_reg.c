#include "status_reg.h"

#include <stddef.h>

/* TODO: implement. Defining the field positions and widths as constants first will help. */

#define REG_READY_MASK      0x01u
#define REG_ERROR_MASK      0x02u
#define REG_OVERRUN_MASK    0x04u
#define REG_MODE_MASK       0x0Fu
#define REG_MODE_SHIFT      4u
#define REG_FIFO_MASK       0xFFu
#define REG_FIFO_SHIFT      8u
#define REG_TEMP_MASK       0xFFFu
#define REG_TEMP_SHIFT      16u
#define REG_VERSION_MASK    0x0Fu
#define REG_VERSION_SHIFT   28u

status_t status_decode(uint32_t reg)
{
    status_t s = {0};

    s.ready = (reg & REG_READY_MASK) != 0;
    s.error = (reg & REG_ERROR_MASK) != 0;
    s.overrun = (reg & REG_OVERRUN_MASK) != 0;

    uint32_t mode_val = (reg >> REG_MODE_SHIFT) & REG_MODE_MASK;
    if (mode_val <= 3u) {
        s.mode = (dev_mode_t)mode_val;
    } else {
        s.mode = DEV_MODE_INVALID;
    }

    s.fifo_level = (uint8_t)((reg >> REG_FIFO_SHIFT) & REG_FIFO_MASK);

    uint32_t raw_temp = (reg >> REG_TEMP_SHIFT) & REG_TEMP_MASK;
    int16_t temp = (int16_t)raw_temp;
    if ((raw_temp & 0x800u) != 0) {
        temp |= (int16_t)0xF000;
    }
    s.temp_sixteenths = temp;

    s.version = (uint8_t)((reg >> REG_VERSION_SHIFT) & REG_VERSION_MASK);

    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }

    uint32_t reg = 0u;

    if (s->ready) {
        reg |= REG_READY_MASK;
    }
    if (s->error) {
        reg |= REG_ERROR_MASK;
    }
    if (s->overrun) {
        reg |= REG_OVERRUN_MASK;
    }

    uint32_t mode_val = 0u;
    if (s->mode >= DEV_MODE_IDLE && s->mode <= DEV_MODE_SLEEP) {
        mode_val = (uint32_t)s->mode;
    } else {
        mode_val = 0xFu;
    }
    reg |= (mode_val & REG_MODE_MASK) << REG_MODE_SHIFT;

    reg |= ((uint32_t)s->fifo_level & REG_FIFO_MASK) << REG_FIFO_SHIFT;

    int16_t t = s->temp_sixteenths;
    if (t < -2048) {
        t = -2048;
    } else if (t > 2047) {
        t = 2047;
    }
    uint32_t raw_temp = (uint32_t)t & REG_TEMP_MASK;
    reg |= raw_temp << REG_TEMP_SHIFT;

    reg |= ((uint32_t)s->version & REG_VERSION_MASK) << REG_VERSION_SHIFT;

    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL) {
        return status_decode(0u);
    }
    uint32_t val = *reg;
    return status_decode(val);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }
    int32_t sixteenths = (int32_t)s->temp_sixteenths;
    
    // Round to nearest, halves away from zero
    if (sixteenths >= 0) {
        sixteenths = (sixteenths + 8) / 16;
    } else {
        sixteenths = (sixteenths - 8) / 16;
    }
    return (int16_t)sixteenths;
}