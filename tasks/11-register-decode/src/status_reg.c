#include "status_reg.h"
#include <stddef.h>
#define SHIFT_RDY           0u
#define SHIFT_ERR           1u
#define SHIFT_OVR           2u
#define SHIFT_MODE          4u
#define SHIFT_FIFO          8u
#define SHIFT_TEMP          16u
#define SHIFT_VER           28u
#define MASK_BIT            0x1u
#define MASK_MODE           0xFu
#define MASK_FIFO           0xFFu
#define MASK_TEMP           0xFFFu
#define MASK_VER            0xFu
#define TEMP_MIN            (-2048)
#define TEMP_MAX            2047

status_t status_decode(uint32_t reg)
{
    status_t s;
    s.ready = (bool)((reg >> SHIFT_RDY) & MASK_BIT);
    s.error = (bool)((reg >> SHIFT_ERR) & MASK_BIT);
    s.overrun = (bool)((reg >> SHIFT_OVR) & MASK_BIT);
    uint8_t raw_mode = (uint8_t)((reg >> SHIFT_MODE) & MASK_MODE);
    if (raw_mode <= (uint8_t)DEV_MODE_SLEEP) {
        s.mode = (dev_mode_t)raw_mode;
    } else {
        s.mode = DEV_MODE_INVALID;
    }
    s.fifo_level = (uint8_t)((reg >> SHIFT_FIFO) & MASK_FIFO);
    uint16_t raw_temp = (uint16_t)((reg >> SHIFT_TEMP) & MASK_TEMP);
    int16_t temp = (int16_t)(raw_temp << 4);
    s.temp_sixteenths = (int16_t)(temp >> 4);
    s.version = (uint8_t)((reg >> SHIFT_VER) & MASK_VER);
    return s;
}

uint32_t status_encode(const status_t *s)
{
    if (s == NULL) {
        return 0u;
    }
    uint32_t reg = 0u;
    if (s->ready) {
        reg |= (1u << SHIFT_RDY);
    }
    if (s->error) {
        reg |= (1u << SHIFT_ERR);
    }
    if (s->overrun) {
        reg |= (1u << SHIFT_OVR);
    }
    uint32_t mode_val;
    if (s->mode >= DEV_MODE_IDLE && s->mode <= DEV_MODE_SLEEP) {
        mode_val = (uint32_t)s->mode;
    } else {
        mode_val = MASK_MODE;
    }
    reg |= ((mode_val & MASK_MODE) << SHIFT_MODE);
    reg |= (((uint32_t)s->fifo_level & MASK_FIFO) << SHIFT_FIFO);
    int16_t t = s->temp_sixteenths;
    if (t < TEMP_MIN) {
        t = TEMP_MIN;
    }
    if (t > TEMP_MAX) {
        t = TEMP_MAX;
    }
    uint32_t raw_temp = ((uint32_t)(uint16_t)t) & MASK_TEMP;
    reg |= (raw_temp << SHIFT_TEMP);
    reg |= (((uint32_t)s->version & MASK_VER) << SHIFT_VER);
    return reg;
}

status_t status_read(const volatile uint32_t *reg)
{
    if (reg == NULL) {
        return status_decode(0u);
    }
    uint32_t val = *reg;
    return status_decode(val);
}

int16_t status_temp_celsius(const status_t *s)
{
    if (s == NULL) {
        return 0;
    }
    int16_t sixteenths = s->temp_sixteenths;
    if (sixteenths >= 0) {
        return (int16_t)((sixteenths + 8) / 16);
    } else {
        return (int16_t)((sixteenths - 8) / 16);
    }
}