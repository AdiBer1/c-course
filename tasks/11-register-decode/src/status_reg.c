#include "status_reg.h"

#include <stddef.h>

/* ------------------------------------------------------------------
 * Field positions and masks (mask = field width in ones, before shift)
 * ------------------------------------------------------------------ */
#define FLAG_MASK          0x1u

#define READY_POS          0u
#define ERROR_POS          1u
#define OVERRUN_POS        2u
/* bit 3 is reserved: ignored on decode, written as 0 on encode */

#define MODE_POS           4u
#define MODE_MASK          0xFu
#define MODE_INVALID_FIELD 0xFu /* value written for an invalid mode */

#define FIFO_POS           8u
#define FIFO_MASK          0xFFu

#define TEMP_POS           16u
#define TEMP_MASK          0xFFFu  /* 12 bits */
#define TEMP_SIGN_BIT      0x800u  /* bit 11 inside the field */
#define TEMP_RANGE         0x1000  /* 2^12, subtracted to sign-extend */
#define TEMP_MIN           (-2048)
#define TEMP_MAX           2047

#define VERSION_POS        28u
#define VERSION_MASK       0xFu

#define SIXTEENTHS_PER_DEG 16
#define HALF_DEG           8

/* Extract a field: shift it down to bit 0, then keep only its bits. */
static uint32_t get_field(uint32_t reg, uint32_t pos, uint32_t mask)
{
    return (reg >> pos) & mask;
}

status_t status_decode(uint32_t reg)
{
    status_t s = {0};

    s.ready = get_field(reg, READY_POS, FLAG_MASK) != 0u;
    s.error = get_field(reg, ERROR_POS, FLAG_MASK) != 0u;
    s.overrun = get_field(reg, OVERRUN_POS, FLAG_MASK) != 0u;

    uint32_t mode = get_field(reg, MODE_POS, MODE_MASK);
    s.mode = (mode <= (uint32_t)DEV_MODE_SLEEP) ? (dev_mode_t)mode : DEV_MODE_INVALID;

    s.fifo_level = (uint8_t)get_field(reg, FIFO_POS, FIFO_MASK);

    /* Sign extension: if bit 11 is set, the 12-bit value is negative,
     * so its real value is raw - 2^12. */
    uint32_t raw_temp = get_field(reg, TEMP_POS, TEMP_MASK);
    int32_t temp = (int32_t)raw_temp;
    if ((raw_temp & TEMP_SIGN_BIT) != 0u) {
        temp -= TEMP_RANGE;
    }
    s.temp_sixteenths = (int16_t)temp;

    s.version = (uint8_t)get_field(reg, VERSION_POS, VERSION_MASK);

    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }

    uint32_t reg = 0u;

    if (s->ready) {
        reg |= FLAG_MASK << READY_POS;
    }
    if (s->error) {
        reg |= FLAG_MASK << ERROR_POS;
    }
    if (s->overrun) {
        reg |= FLAG_MASK << OVERRUN_POS;
    }

    uint32_t mode;
    switch (s->mode) {
    case DEV_MODE_IDLE:
    case DEV_MODE_MEASURE:
    case DEV_MODE_CALIBRATE:
    case DEV_MODE_SLEEP:
        mode = (uint32_t)s->mode;
        break;
    default: /* DEV_MODE_INVALID or any out-of-range value */
        mode = MODE_INVALID_FIELD;
        break;
    }
    reg |= (mode & MODE_MASK) << MODE_POS;

    reg |= ((uint32_t)s->fifo_level & FIFO_MASK) << FIFO_POS;

    int32_t temp = s->temp_sixteenths;
    if (temp < TEMP_MIN) {
        temp = TEMP_MIN;
    } else if (temp > TEMP_MAX) {
        temp = TEMP_MAX;
    }
    /* The cast keeps the two's complement bits; the mask keeps the low 12. */
    reg |= ((uint32_t)temp & TEMP_MASK) << TEMP_POS;

    reg |= ((uint32_t)s->version & VERSION_MASK) << VERSION_POS;

    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    uint32_t snapshot = 0u;
    if (reg != NULL) {
        snapshot = *reg; /* the one and only hardware read */
    }
    return status_decode(snapshot);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }

    int32_t t = s->temp_sixteenths;
    int32_t deg;
    /* C division truncates toward zero, so add/subtract half a degree
     * first to get round-half-away-from-zero. */
    if (t >= 0) {
        deg = (t + HALF_DEG) / SIXTEENTHS_PER_DEG;
    } else {
        deg = (t - HALF_DEG) / SIXTEENTHS_PER_DEG;
    }
    return (int16_t)deg;
}
