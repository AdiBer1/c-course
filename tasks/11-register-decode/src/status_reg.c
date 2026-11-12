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
        temp |= (int16_t)0xF000; /* Sign extension for 12-bit two's complement */
    }
    s.temp_sixteenths = temp;

    s.version = (uint8_t)((reg >> REG_VERSION_SHIFT) & REG_VERSION_MASK);

    return s;
}

uint32_t status_encode(const status_t *s)
{
    (void)s;
    return 0;
}

status_t status_read(const volatile uint32_t *reg)
{
    (void)reg;
    status_t s = {0};
    return s;
}

int16_t status_temp_celsius(const status_t *s)
{
    (void)s;
    return 0;
}