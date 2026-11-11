#include "convert.h"

/*
 * Divide num by den (den > 0), rounding to nearest, halves away from zero.
 * C division truncates toward zero, so for a negative numerator we must
 * subtract half the divisor instead of adding it.
 */
static int32_t div_round(int32_t num, int32_t den)
{
    if (num >= 0) {
        return (num + den / 2) / den;
    }
    return (num - den / 2) / den;
}

int32_t c10_to_f10(int32_t c10)
{
    return div_round(c10 * 9, 5) + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    return div_round((f10 - 320) * 5, 9);
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095U) {
        raw = 4095U;
    }
    uint32_t mv = ((uint32_t)raw * 3300U + 4095U / 2U) / 4095U;
    return (uint16_t)mv;
}
