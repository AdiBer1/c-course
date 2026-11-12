#include "status_reg.h"
#define READY_BIT 0u
#define ERROR_BIT 1u
#define OVERRUN_BIT 2u
#define MODE_POS 4u
#define MODE_MASK 0xFu
#define FIFO_POS 8u
#define FIFO_MASK 0xFFu
#define TEMP_POS 16u
#define TEMP_MASK 0xFFFu
#define VER_POS 28u
#define VER_MASK 0xFu
#include <stddef.h>

/* TODO: implement. Defining the field positions and widths as constants first will help. */

status_t status_decode(uint32_t reg)
{
    status_t s;

    s.ready = ((reg >> READY_BIT) & 1u) != 0u;
    s.error = ((reg >> ERROR_BIT) & 1u) != 0u;
    s.overrun = ((reg >> OVERRUN_BIT) & 1u) != 0u;

    uint32_t mode = (reg >> MODE_POS) & MODE_MASK;
    if (mode <= 3u) {
        s.mode = (dev_mode_t)mode;
    } else {
        s.mode = DEV_MODE_INVALID;
    }

    s.fifo_level = (uint8_t)((reg >> FIFO_POS) & FIFO_MASK);

    int32_t temp = (int32_t)((reg >> TEMP_POS) & TEMP_MASK);
    if (temp >= 2048) {
        temp = temp - 4096;
    }
    s.temp_sixteenths = (int16_t)temp;

    s.version = (uint8_t)((reg >> VER_POS) & VER_MASK);

    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }

    uint32_t reg = 0u;

    if (s->ready) {
        reg |= 1u << READY_BIT;
    }
    if (s->error) {
        reg |= 1u << ERROR_BIT;
    }
    if (s->overrun) {
        reg |= 1u << OVERRUN_BIT;
    }

    uint32_t mode = (uint32_t)s->mode;
    if (mode > 3u) {
        mode = 0xFu;
    }
    reg |= (mode & MODE_MASK) << MODE_POS;

    reg |= ((uint32_t)s->fifo_level & FIFO_MASK) << FIFO_POS;

    int32_t temp = s->temp_sixteenths;
    if (temp > 2047) {
        temp = 2047;
    }
    if (temp < -2048) {
        temp = -2048;
    }
    reg |= ((uint32_t)temp & TEMP_MASK) << TEMP_POS;

    reg |= ((uint32_t)s->version & VER_MASK) << VER_POS;

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

    int32_t t = s->temp_sixteenths;
    int32_t result;
    if (t >= 0) {
        result = (t + 8) / 16;
    } else {
        result = (t - 8) / 16;
    }
    return (int16_t)result;
}