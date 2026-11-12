#include "status_reg.h"

#include <stddef.h>

#define REG_READY_SHIFT    0u
#define REG_ERROR_SHIFT    1u
#define REG_OVERRUN_SHIFT  2u
#define REG_MODE_SHIFT     4u
#define REG_FIFO_SHIFT     8u
#define REG_TEMP_SHIFT     16u
#define REG_VERSION_SHIFT  28u

#define REG_BIT_MASK       0x1u
#define REG_MODE_MASK      0xFu
#define REG_FIFO_MASK      0xFFu
#define REG_TEMP_MASK      0xFFFu
#define REG_VERSION_MASK   0xFu

#define TEMP_SIGN_BIT      0x0800u
#define TEMP_EXTEND_MASK   0xF000u
#define TEMP_MIN           (-2048)
#define TEMP_MAX           2047
#define MODE_INVALID_RAW   0xFu

status_t status_decode(uint32_t reg)
{
    status_t s;

    s.ready = ((reg >> REG_READY_SHIFT) & REG_BIT_MASK) != 0u;
    s.error = ((reg >> REG_ERROR_SHIFT) & REG_BIT_MASK) != 0u;
    s.overrun = ((reg >> REG_OVERRUN_SHIFT) & REG_BIT_MASK) != 0u;

    uint32_t raw_mode = (reg >> REG_MODE_SHIFT) & REG_MODE_MASK;
    if (raw_mode <= (uint32_t)DEV_MODE_SLEEP) {
        s.mode = (dev_mode_t)raw_mode;
    } else {
        s.mode = DEV_MODE_INVALID;
    }

    s.fifo_level = (uint8_t)((reg >> REG_FIFO_SHIFT) & REG_FIFO_MASK);

    uint16_t raw_temp = (uint16_t)((reg >> REG_TEMP_SHIFT) & REG_TEMP_MASK);
    if ((raw_temp & TEMP_SIGN_BIT) != 0u) {
        raw_temp = (uint16_t)(raw_temp | TEMP_EXTEND_MASK);
    }
    s.temp_sixteenths = (int16_t)raw_temp;

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
        reg |= (1u << REG_READY_SHIFT);
    }
    if (s->error) {
        reg |= (1u << REG_ERROR_SHIFT);
    }
    if (s->overrun) {
        reg |= (1u << REG_OVERRUN_SHIFT);
    }

    uint32_t raw_mode;
    if (s->mode >= DEV_MODE_IDLE && s->mode <= DEV_MODE_SLEEP) {
        raw_mode = (uint32_t)s->mode;
    } else {
        raw_mode = MODE_INVALID_RAW;
    }
    reg |= (raw_mode & REG_MODE_MASK) << REG_MODE_SHIFT;

    reg |= ((uint32_t)s->fifo_level & REG_FIFO_MASK) << REG_FIFO_SHIFT;

    int16_t clamped_temp = s->temp_sixteenths;
    if (clamped_temp < TEMP_MIN) {
        clamped_temp = TEMP_MIN;
    } else if (clamped_temp > TEMP_MAX) {
        clamped_temp = TEMP_MAX;
    }
    uint32_t raw_temp = (uint32_t)((uint16_t)clamped_temp) & REG_TEMP_MASK;
    reg |= raw_temp << REG_TEMP_SHIFT;

    reg |= ((uint32_t)s->version & REG_VERSION_MASK) << REG_VERSION_SHIFT;

    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL) {
        return status_decode(0u);
    }

    uint32_t raw = *reg;
    return status_decode(raw);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }

    int32_t t = (int32_t)s->temp_sixteenths;
    if (t >= 0) {
        return (int16_t)((t + 8) / 16);
    } else {
        return (int16_t)((t - 8) / 16);
    }
}