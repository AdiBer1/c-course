#include "status_reg.h"

#include <stddef.h>

/* Bit positions */
#define READY_POS       0u
#define ERROR_POS       1u
#define OVERRUN_POS     2u
#define MODE_POS        4u
#define FIFO_POS        8u
#define TEMP_POS        16u
#define VERSION_POS     28u

/* Field masks */
#define BIT_MASK        0x01u
#define MODE_MASK       0x0Fu
#define BYTE_MASK       0xFFu
#define TEMP_MASK       0x0FFFu
#define VERSION_MASK    0x0Fu

/* Temperature is a signed 12-bit two's complement value */
#define TEMP_SIGN_BIT   0x0800u
#define TEMP_RANGE      0x1000


status_t status_decode(uint32_t reg)
{
    status_t s = {0};

    /* Decode individual flags */
    s.ready = ((reg >> READY_POS) & BIT_MASK) != 0u;
    s.error = ((reg >> ERROR_POS) & BIT_MASK) != 0u;
    s.overrun = ((reg >> OVERRUN_POS) & BIT_MASK) != 0u;

    /* Decode mode */
    uint32_t mode = (reg >> MODE_POS) & MODE_MASK;

    if (mode <= (uint32_t)DEV_MODE_SLEEP) {
        s.mode = (dev_mode_t)mode;
    } else {
        s.mode = DEV_MODE_INVALID;
    }

    /* Decode FIFO level */
    s.fifo_level = (uint8_t)((reg >> FIFO_POS) & BYTE_MASK);

    /* Decode signed 12-bit temperature */
    uint32_t raw_temp = (reg >> TEMP_POS) & TEMP_MASK;
    int32_t temp = (int32_t)raw_temp;

    if ((raw_temp & TEMP_SIGN_BIT) != 0u) {
        temp -= TEMP_RANGE;
    }

    s.temp_sixteenths = (int16_t)temp;

    /* Decode chip version */
    s.version = (uint8_t)((reg >> VERSION_POS) & VERSION_MASK);

    return s;
}


uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }

    uint32_t reg = 0u;

    /* Encode flags */
    if (s->ready) {
        reg |= (BIT_MASK << READY_POS);
    }

    if (s->error) {
        reg |= (BIT_MASK << ERROR_POS);
    }

    if (s->overrun) {
        reg |= (BIT_MASK << OVERRUN_POS);
    }

    /* Encode mode */
    uint32_t mode;

    if (s->mode >= DEV_MODE_IDLE && s->mode <= DEV_MODE_SLEEP) {
        mode = (uint32_t)s->mode;
    } else {
        mode = MODE_MASK;
    }

    reg |= (mode << MODE_POS);

    /* Encode FIFO level */
    reg |= ((uint32_t)s->fifo_level << FIFO_POS);

    /* Clamp temperature to the signed 12-bit range */
    int32_t temp = (int32_t)s->temp_sixteenths;

    if (temp > 2047) {
        temp = 2047;
    } else if (temp < -2048) {
        temp = -2048;
    }

    /* Keep the low 12 bits of the two's complement value */
    uint32_t raw_temp = (uint32_t)temp & TEMP_MASK;
    reg |= (raw_temp << TEMP_POS);

    /* Encode only the low 4 version bits */
    reg |= (((uint32_t)s->version & VERSION_MASK) << VERSION_POS);

    /* Reserved bit 3 remains zero */
    return reg;
}


status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL) {
        return status_decode(0u);
    }

    /* Read the hardware register exactly once */
    uint32_t value = *reg;

    return status_decode(value);
}


int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }

    int32_t temp = (int32_t)s->temp_sixteenths;

    /* Round to nearest whole degree, halves away from zero */
    if (temp >= 0) {
        temp = (temp + 8) / 16;
    } else {
        temp = -((-temp + 8) / 16);
    }

    return (int16_t)temp;
}
