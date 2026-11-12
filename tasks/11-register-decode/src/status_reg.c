#include "status_reg.h"
#include <stddef.h>

/* Field definitions: position and width */
#define RDY_BIT         0u
#define ERR_BIT         1u
#define OVR_BIT         2u
#define MODE_POS        4u
#define MODE_WIDTH      4u
#define FIFO_POS        8u
#define FIFO_WIDTH      8u
#define TEMP_POS        16u
#define TEMP_WIDTH      12u
#define VERSION_POS     28u
#define VERSION_WIDTH   4u

status_t status_decode(uint32_t reg)
{
    status_t s;
    
    s.ready = (bool)((reg >> RDY_BIT) & 1u);
    s.error = (bool)((reg >> ERR_BIT) & 1u);
    s.overrun = (bool)((reg >> OVR_BIT) & 1u);

    uint32_t raw_mode = (reg >> MODE_POS) & ((1u << MODE_WIDTH) - 1u);
    if (raw_mode == 0u) {
        s.mode = DEV_MODE_IDLE;
    } else if (raw_mode == 1u) {
        s.mode = DEV_MODE_MEASURE;
    } else if (raw_mode == 2u) {
        s.mode = DEV_MODE_CALIBRATE;
    } else if (raw_mode == 3u) {
        s.mode = DEV_MODE_SLEEP;
    } else {
        s.mode = DEV_MODE_INVALID;
    }

    s.fifo_level = (uint8_t)((reg >> FIFO_POS) & ((1u << FIFO_WIDTH) - 1u));

    /* 12-bit two's complement temperature extraction with sign extension */
    uint32_t raw_temp = (reg >> TEMP_POS) & ((1u << TEMP_WIDTH) - 1u);
    if (raw_temp & (1u << (TEMP_WIDTH - 1u))) {
        raw_temp |= 0xF000u; /* Extend sign to 16-bit */
    }
    s.temp_sixteenths = (int16_t)raw_temp;

    s.version = (uint8_t)((reg >> VERSION_POS) & ((1u << VERSION_WIDTH) - 1u));

    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }

    uint32_t reg = 0u;

    if (s->ready) {
        reg |= (1u << RDY_BIT);
    }
    if (s->error) {
        reg |= (1u << ERR_BIT);
    }
    if (s->overrun) {
        reg |= (1u << OVR_BIT);
    }

    uint32_t mode_val = 0u;
    switch (s->mode) {
        case DEV_MODE_IDLE:      mode_val = 0u; break;
        case DEV_MODE_MEASURE:   mode_val = 1u; break;
        case DEV_MODE_CALIBRATE: mode_val = 2u; break;
        case DEV_MODE_SLEEP:     mode_val = 3u; break;
        default:                 mode_val = 0xFu; break; /* invalid or out of range -> 0xF */
    }
    reg |= (mode_val & ((1u << MODE_WIDTH) - 1u)) << MODE_POS;

    reg |= ((uint32_t)s->fifo_level & ((1u << FIFO_WIDTH) - 1u)) << FIFO_POS;

    /* Clamp temperature to -2048 .. 2047 */
    int16_t t = s->temp_sixteenths;
    if (t > 2047) {
        t = 2047;
    } else if (t < -2048) {
        t = -2048;
    }
    uint32_t temp_val = (uint32_t)((uint16_t)t & ((1u << TEMP_WIDTH) - 1u));
    reg |= temp_val << TEMP_POS;

    reg |= ((uint32_t)s->version & ((1u << VERSION_WIDTH) - 1u)) << VERSION_POS;

    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL) {
        return status_decode(0u);
    }
    /* Read exactly once from the volatile pointer */
    uint32_t val = *reg;
    return status_decode(val);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }
    /* Convert temp_sixteenths (1/16 deg C) to whole degrees C, rounded to nearest */
    int32_t t16 = (int32_t)s->temp_sixteenths;
    if (t16 >= 0) {
        return (int16_t)((t16 + 8) / 16);
    } else {
        return (int16_t)((t16 - 8) / 16);
    }
}