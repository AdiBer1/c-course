
#include "convert.h"

/* Divide and round to the nearest integer.
   Half values are rounded away from zero. */
static int32_t divide_round(int32_t numerator, int32_t denominator)
{
    if (numerator >= 0)
    {
        return (numerator + denominator / 2) / denominator;
    }

    return (numerator - denominator / 2) / denominator;
}

int32_t c10_to_f10(int32_t c10)
{
    int32_t numerator = c10 * 9;
    return divide_round(numerator, 5) + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    int32_t numerator = (f10 - 320) * 5;
    return divide_round(numerator, 9);
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095)
    {
        raw = 4095;
    }

    uint32_t numerator = (uint32_t)raw * 3300U;

    return (uint16_t)((numerator + 4095U / 2U) / 4095U);
}
