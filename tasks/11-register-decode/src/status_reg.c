#include "status_reg.h"
#include <stddef.h>

status_t status_decode(uint32_t reg)
{
    status_t s = {0};

    s.ready = (reg & 1u) != 0;
    s.error = (reg & 2u) != 0;
    s.overrun = (reg & 4u) != 0;

    uint32_t mode_value = (reg >> 4) & 0xFu;

    if (mode_value <= 3u) {
        s.mode = (dev_mode_t)mode_value;
    } else {
        s.mode = DEV_MODE_INVALID;
    }

    s.fifo_level = (uint8_t)((reg >> 8) & 0xFFu);

    uint32_t raw_temp = (reg >> 16) & 0xFFFu;

    if ((raw_temp & 0x800u) != 0) {
        s.temp_sixteenths = (int16_t)((int32_t)raw_temp - 4096);
    } else {
        s.temp_sixteenths = (int16_t)raw_temp;
    }

    s.version = (uint8_t)((reg >> 28) & 0xFu);

    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }

    uint32_t reg = 0;

    if (s->ready) {
        reg |= 1u;
    }

    if (s->error) {
        reg |= 2u;
    }

    if (s->overrun) {
        reg |= 4u;
    }

    uint32_t mode_value;

    if (s->mode >= DEV_MODE_IDLE && s->mode <= DEV_MODE_SLEEP) {
        mode_value = (uint32_t)s->mode;
    } else {
        mode_value = 0xFu;
    }

    reg |= mode_value << 4;
    reg |= (uint32_t)s->fifo_level << 8;

    int32_t temp = s->temp_sixteenths;

    if (temp > 2047) {
        temp = 2047;
    } else if (temp < -2048) {
        temp = -2048;
    }

    reg |= ((uint32_t)temp & 0xFFFu) << 16;
    reg |= ((uint32_t)s->version & 0xFu) << 28;

    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL) {
        return status_decode(0);
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

    return (int16_t)((temp - 8) / 16);
}
