#include "status_reg.h"

#include <stddef.h>

/* ------------------------------------------------------- register layout */
/* Every field is described once: its position (lowest bit) and its mask
 * (after shifting down to bit 0). */

#define READY_POS   0u
#define ERROR_POS   1u
#define OVERRUN_POS 2u
/* bit 3 is reserved: ignored on decode, written as 0 on encode */

#define MODE_POS  4u
#define MODE_MASK 0xFu /* 4 bits */

#define FIFO_POS  8u
#define FIFO_MASK 0xFFu /* 8 bits */

#define TEMP_POS      16u
#define TEMP_MASK     0xFFFu /* 12 bits */
#define TEMP_SIGN_BIT 0x800u /* bit 11 of the field: the two's complement sign */
#define TEMP_RANGE    4096   /* 2^12 */
#define TEMP_MIN      (-2048)
#define TEMP_MAX      2047

#define VERSION_POS  28u
#define VERSION_MASK 0xFu /* 4 bits */

#define MODE_INVALID_RAW 0xFu /* what an invalid mode is encoded as */

/* Extract a field: shift it down to bit 0, then keep only its bits. */
static uint32_t field_get(uint32_t reg, uint32_t pos, uint32_t mask)
{
    return (reg >> pos) & mask;
}

/* ------------------------------------------------------------------- API */

status_t status_decode(uint32_t reg)
{
    status_t s;

    s.ready = field_get(reg, READY_POS, 1u) != 0u;
    s.error = field_get(reg, ERROR_POS, 1u) != 0u;
    s.overrun = field_get(reg, OVERRUN_POS, 1u) != 0u;

    uint32_t mode = field_get(reg, MODE_POS, MODE_MASK);
    s.mode = (mode <= (uint32_t)DEV_MODE_SLEEP) ? (dev_mode_t)mode : DEV_MODE_INVALID;

    s.fifo_level = (uint8_t)field_get(reg, FIFO_POS, FIFO_MASK);

    /*
     * Sign extension of the 12-bit two's complement temperature.
     * The raw field is 0..4095. If its top bit (bit 11) is set, the number
     * is negative and its real value is raw - 4096. Example:
     * 0xF60 = 3936, bit 11 set -> 3936 - 4096 = -160.
     */
    uint32_t raw_temp = field_get(reg, TEMP_POS, TEMP_MASK);
    int32_t temp = (int32_t)raw_temp;
    if ((raw_temp & TEMP_SIGN_BIT) != 0u) {
        temp -= TEMP_RANGE;
    }
    s.temp_sixteenths = (int16_t)temp; /* -2048 .. 2047, always fits */

    s.version = (uint8_t)field_get(reg, VERSION_POS, VERSION_MASK);

    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }

    uint32_t reg = 0u; /* reserved bit 3 stays 0 */

    if (s->ready) {
        reg |= 1u << READY_POS;
    }
    if (s->error) {
        reg |= 1u << ERROR_POS;
    }
    if (s->overrun) {
        reg |= 1u << OVERRUN_POS;
    }

    uint32_t mode;
    switch (s->mode) {
    case DEV_MODE_IDLE:
    case DEV_MODE_MEASURE:
    case DEV_MODE_CALIBRATE:
    case DEV_MODE_SLEEP:
        mode = (uint32_t)s->mode;
        break;
    case DEV_MODE_INVALID:
    default:
        mode = MODE_INVALID_RAW;
        break;
    }
    reg |= mode << MODE_POS;

    reg |= ((uint32_t)s->fifo_level & FIFO_MASK) << FIFO_POS;

    /* Clamp, then keep the low 12 bits of the two's complement value:
     * converting a negative int32_t to uint32_t is well defined (modulo 2^32),
     * so -1 becomes 0xFFFFFFFF, and & 0xFFF leaves 0xFFF. */
    int32_t temp = s->temp_sixteenths;
    if (temp < TEMP_MIN) {
        temp = TEMP_MIN;
    } else if (temp > TEMP_MAX) {
        temp = TEMP_MAX;
    }
    reg |= ((uint32_t)temp & TEMP_MASK) << TEMP_POS;

    reg |= ((uint32_t)s->version & VERSION_MASK) << VERSION_POS;

    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL) {
        return status_decode(0u);
    }

    /* Exactly ONE read of the hardware register: all fields are decoded
     * from this single snapshot, so they all describe the same moment. */
    uint32_t snapshot = *reg;
    return status_decode(snapshot);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }

    /* 1 LSB = 1/16 degree: divide by 16, rounding to nearest with halves
     * away from zero (push the value half a divisor away from zero first,
     * because C division truncates toward zero). */
    int32_t t = s->temp_sixteenths;
    int32_t c = (t >= 0) ? (t + 8) / 16 : (t - 8) / 16;
    return (int16_t)c;
}