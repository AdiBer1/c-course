#include "status_reg.h"

#include <stddef.h>

/* Field positions and widths, defined once. */
#define READY_POS    0u
#define ERROR_POS    1u
#define OVERRUN_POS  2u
#define MODE_POS     4u
#define MODE_WIDTH   4u
#define FIFO_POS     8u
#define FIFO_WIDTH   8u
#define TEMP_POS     16u
#define TEMP_WIDTH   12u
#define VERSION_POS  28u
#define VERSION_WIDTH 4u

#define MASK(width)            ((uint32_t)(0xFFFFFFFFu >> (32u - (width))))
#define GET(reg, pos, width)   (((reg) >> (pos)) & MASK(width))
#define PUT(val, pos, width)   (((uint32_t)(val) & MASK(width)) << (pos))

#define MODE_INVALID_RAW 0xFu
#define TEMP_MIN         (-2048)
#define TEMP_MAX         2047
#define TEMP_SIGN_BIT    0x800u   /* bit 11 of the 12-bit field */
#define TEMP_RANGE       4096     /* 2^12 */

status_t status_decode(uint32_t reg)
{
    status_t s;

    s.ready = GET(reg, READY_POS, 1u) != 0u;
    s.error = GET(reg, ERROR_POS, 1u) != 0u;
    s.overrun = GET(reg, OVERRUN_POS, 1u) != 0u;

    uint32_t mode = GET(reg, MODE_POS, MODE_WIDTH);
    s.mode = (mode <= (uint32_t)DEV_MODE_SLEEP) ? (dev_mode_t)mode : DEV_MODE_INVALID;

    s.fifo_level = (uint8_t)GET(reg, FIFO_POS, FIFO_WIDTH);

    /* 12-bit two's complement: if the sign bit is set, the value is raw - 4096. */
    int32_t temp = (int32_t)GET(reg, TEMP_POS, TEMP_WIDTH);
    if (((uint32_t)temp & TEMP_SIGN_BIT) != 0u) {
        temp -= TEMP_RANGE;
    }
    s.temp_sixteenths = (int16_t)temp;

    s.version = (uint8_t)GET(reg, VERSION_POS, VERSION_WIDTH);
    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }

    uint32_t reg = 0u;
    reg |= PUT(s->ready ? 1u : 0u, READY_POS, 1u);
    reg |= PUT(s->error ? 1u : 0u, ERROR_POS, 1u);
    reg |= PUT(s->overrun ? 1u : 0u, OVERRUN_POS, 1u);

    uint32_t mode = (uint32_t)s->mode;
    if (mode > (uint32_t)DEV_MODE_SLEEP) {
        mode = MODE_INVALID_RAW;
    }
    reg |= PUT(mode, MODE_POS, MODE_WIDTH);

    reg |= PUT(s->fifo_level, FIFO_POS, FIFO_WIDTH);

    int32_t temp = s->temp_sixteenths;
    if (temp < TEMP_MIN) {
        temp = TEMP_MIN;
    } else if (temp > TEMP_MAX) {
        temp = TEMP_MAX;
    }
    /* Converting a negative value to uint32_t gives its two's complement;
       PUT keeps only the low 12 bits of it. */
    reg |= PUT((uint32_t)temp, TEMP_POS, TEMP_WIDTH);

    reg |= PUT(s->version, VERSION_POS, VERSION_WIDTH);
    return reg; /* the reserved bit 3 stays 0 */
}

status_t status_read(const volatile uint32_t *reg)
{
    uint32_t value = 0u;
    if (reg != NULL) {
        value = *reg; /* the ONE read of the hardware register */
    }
    return status_decode(value);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }
    int32_t t = s->temp_sixteenths;
    int32_t c;
    if (t >= 0) {
        c = (t + 8) / 16;          /* +8 = half of 16: rounds halves up */
    } else {
        c = -((-t + 8) / 16);      /* mirror image: halves away from zero */
    }
    return (int16_t)c;
}
