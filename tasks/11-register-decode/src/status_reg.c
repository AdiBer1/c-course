#include "status_reg.h"

#include <stddef.h>

#define POS_RDY   0
#define POS_ERR   1
#define POS_OVR   2
#define POS_MODE  4
#define POS_FIFO  8
#define POS_TEMP  16
#define POS_VER   28

#define MASK_RDY  0x1u
#define MASK_ERR  0x1u
#define MASK_OVR  0x1u
#define MASK_MODE 0xFu
#define MASK_FIFO 0xFFu
#define MASK_TEMP 0xFFFu
#define MASK_VER  0xFu

status_t status_decode(uint32_t reg)
{
    status_t s;
    s.ready      = (reg >> POS_RDY) & MASK_RDY;
    s.error      = (reg >> POS_ERR) & MASK_ERR;
    s.overrun    = (reg >> POS_OVR) & MASK_OVR;
    uint8_t mode = (uint8_t)((reg >> POS_MODE) & MASK_MODE);
    s.mode       = (mode <= 3) ? (dev_mode_t)mode : DEV_MODE_INVALID;
    s.fifo_level = (uint8_t)((reg >> POS_FIFO) & MASK_FIFO);
    /* sign-extend 12-bit two's complement */
    uint32_t raw12 = (reg >> POS_TEMP) & MASK_TEMP;
    if (raw12 & 0x800u)
        s.temp_sixteenths = (int16_t)(raw12 | 0xF000u);
    else
        s.temp_sixteenths = (int16_t)raw12;
    s.version    = (uint8_t)((reg >> POS_VER) & MASK_VER);
    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (!s) return 0;
    int16_t temp = s->temp_sixteenths;
    if (temp > 2047)  temp = 2047;
    if (temp < -2048) temp = -2048;
    uint32_t mode_val;
    if (s->mode == DEV_MODE_INVALID || (unsigned)s->mode > 3)
        mode_val = 0xFu;
    else
        mode_val = (uint32_t)s->mode;
    return ((uint32_t)s->ready    << POS_RDY)
         | ((uint32_t)s->error    << POS_ERR)
         | ((uint32_t)s->overrun  << POS_OVR)
         | (mode_val              << POS_MODE)
         | ((uint32_t)s->fifo_level << POS_FIFO)
         | (((uint32_t)(uint16_t)temp & MASK_TEMP) << POS_TEMP)
         | ((uint32_t)s->version  << POS_VER);
}

status_t status_read(const volatile uint32_t *reg)
{
    uint32_t val = reg ? *reg : 0u;
    return status_decode(val);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (!s) return 0;
    int32_t t = s->temp_sixteenths;
    if (t >= 0)
        return (int16_t)((t + 8) / 16);
    else
        return (int16_t)((t - 8) / 16);
}
