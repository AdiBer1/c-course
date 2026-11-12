#include "convert.h"

/* TODO: implement every function. Each one currently returns 0. */

static int32_t div_round(int32_t num, int32_t den)
{
    if ((num < 0) ^ (den < 0)) {
        return (num - den / 2) / den;
    } else {
        return (num + den / 2) / den;
    }
}

int32_t c10_to_f10(int32_t c10)
{
    int32_t temp = c10 * 9;
    int32_t rounded_div = div_round(temp, 5);
    return rounded_div + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    int32_t temp = f10 - 320;
    int32_t prod = temp * 5;
    return div_round(prod, 9);
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095) {
        raw = 4095;
    }

    uint32_t numerator = (uint32_t)raw * 3300;
    uint32_t rounded = (numerator + 4095 / 2) / 4095;
    return (uint16_t)rounded;
}