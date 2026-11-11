
#include "status_reg.h"

#include <stddef.h>


status_t status_decode(uint32_t reg)
{
    status_t s = {0};

    /* Extract version */
    s.version = (reg >> 28) & 0xF;

    /* Extract and sign-extend 12-bit temperature */
    uint32_t raw_temp = (reg >> 16) & 0xFFF;

    if (raw_temp & 0x800)
        s.temp_sixteenths = (int16_t)(raw_temp - 0x1000);
    else
        s.temp_sixteenths = (int16_t)raw_temp;

    /* Extract FIFO level */
    s.fifo_level = (reg >> 8) & 0xFF;

    /* Extract and validate mode */
    uint32_t raw_mode = (reg >> 4) & 0xF;

    if (raw_mode <= DEV_MODE_SLEEP)
        s.mode = (dev_mode_t)raw_mode;
    else
        s.mode = DEV_MODE_INVALID;

    /* Extract status flags */
    s.overrun = (reg >> 2) & 0x1;
    s.error = (reg >> 1) & 0x1;
    s.ready = reg & 0x1;

    return s;
}


uint32_t status_encode(const status_t *s)
{
    if (s == NULL)
        return 0;

    uint32_t reg = 0;

    /* Clamp temperature to signed 12-bit range */
    int32_t temp = s->temp_sixteenths;

    if (temp < -2048)
        temp = -2048;
    else if (temp > 2047)
        temp = 2047;

    /* Validate mode */
    uint32_t mode = (uint32_t)s->mode;

    if (mode > DEV_MODE_SLEEP)
        mode = 0xF;

    /* Pack fields into register */
    reg |= ((uint32_t)(s->version & 0xF) << 28);
    reg |= (((uint32_t)temp & 0xFFF) << 16);
    reg |= ((uint32_t)s->fifo_level << 8);
    reg |= ((mode & 0xF) << 4);
    reg |= ((uint32_t)(s->overrun & 0x1) << 2);
    reg |= ((uint32_t)(s->error & 0x1) << 1);
    reg |= ((uint32_t)(s->ready & 0x1));

    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL)
        return status_decode(0);

    /* Read register exactly once */
    uint32_t raw = *reg;

    return status_decode(raw);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL)
        return 0;

    int32_t temp = s->temp_sixteenths;

    /* Round to nearest, halves away from zero */
    if (temp >= 0)
        return (int16_t)((temp + 8) / 16);

    return (int16_t)((temp - 8) / 16);
}
