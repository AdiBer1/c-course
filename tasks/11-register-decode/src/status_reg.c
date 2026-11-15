#include "status_reg.h"

#include <stddef.h>

/* Field positions and masks, taken from the register layout in status_reg.h. */
#define BIT_READY 0x1u
#define BIT_ERROR 0x2u
#define BIT_OVERRUN 0x4u

#define MODE_SHIFT 4u
#define MODE_MASK 0xFu
#define MODE_FIELD_INVALID 0xFu /* written by status_encode for an invalid mode */

#define FIFO_SHIFT 8u
#define FIFO_MASK 0xFFu

#define TEMP_SHIFT 16u
#define TEMP_MASK 0xFFFu
#define TEMP_SIGN_BIT 0x800u /* bit 11 of the 12-bit field */
#define TEMP_RANGE 0x1000 /* 2^12 */
#define TEMP_MAX 2047
#define TEMP_MIN (-2048)
#define TEMP_LSB_PER_DEGREE 16

#define VERSION_SHIFT 28u
#define VERSION_MASK 0xFu

status_t status_decode(uint32_t reg)
{
    status_t s = {0};

    s.ready = (reg & BIT_READY) != 0u;
    s.error = (reg & BIT_ERROR) != 0u;
    s.overrun = (reg & BIT_OVERRUN) != 0u;

    uint32_t mode = (reg >> MODE_SHIFT) & MODE_MASK;
    if (mode <= (uint32_t)DEV_MODE_SLEEP) {
        s.mode = (dev_mode_t)mode;
    } else {
        s.mode = DEV_MODE_INVALID;
    }

    s.fifo_level = (uint8_t)((reg >> FIFO_SHIFT) & FIFO_MASK);

    /* Sign extension: if bit 11 is set, the value is negative. */
    int32_t temp = (int32_t)((reg >> TEMP_SHIFT) & TEMP_MASK);
    if ((temp & (int32_t)TEMP_SIGN_BIT) != 0) {
        temp -= TEMP_RANGE;
    }
    s.temp_sixteenths = (int16_t)temp;

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
        reg |= BIT_READY;
    }
    if (s->error) {
        reg |= BIT_ERROR;
    }
    if (s->overrun) {
        reg |= BIT_OVERRUN;
    }

    int mode = (int)s->mode;
    uint32_t mode_bits = (mode >= (int)DEV_MODE_IDLE && mode <= (int)DEV_MODE_SLEEP)
                             ? (uint32_t)mode
                             : MODE_FIELD_INVALID;
    reg |= mode_bits << MODE_SHIFT;

    reg |= ((uint32_t)s->fifo_level & FIFO_MASK) << FIFO_SHIFT;

    int32_t temp = s->temp_sixteenths;
    if (temp > TEMP_MAX) {
        temp = TEMP_MAX;
    }
    if (temp < TEMP_MIN) {
        temp = TEMP_MIN;
    }
    /* Negative values wrap to their 12-bit two's complement pattern here. */
    reg |= ((uint32_t)temp & TEMP_MASK) << TEMP_SHIFT;

    reg |= ((uint32_t)s->version & VERSION_MASK) << VERSION_SHIFT;
    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    /* Read the hardware register exactly once; decode the copy. */
    uint32_t raw = (reg != NULL) ? *reg : 0u;
    return status_decode(raw);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }

    /* Round to nearest, halves away from zero: work on the magnitude. */
    int32_t t = s->temp_sixteenths;
    int32_t half = TEMP_LSB_PER_DEGREE / 2;
    int32_t result;
    if (t >= 0) {
        result = (t + half) / TEMP_LSB_PER_DEGREE;
    } else {
        result = -((-t + half) / TEMP_LSB_PER_DEGREE);
    }
    return (int16_t)result;
}