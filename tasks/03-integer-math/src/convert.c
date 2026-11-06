#include "convert.h"

static int32_t div_round(int32_t num, int32_t den)
{
    if (num >= 0) {
        return (num + (den / 2)) / den;
    } else {
        return (num - (den / 2)) / den;
    }
}

int32_t c10_to_f10(int32_t c10)
{
    int32_t scaled = div_round(c10 * 9, 5);
    return scaled + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    int32_t diff = f10 - 320;
    return div_round(diff * 5, 9);
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095) {
        raw = 4095;
    }
    uint32_t val = (uint32_t)raw * 3300;
    uint32_t mv = (val + 2047) / 4095; 
    return (uint16_t)mv;
}