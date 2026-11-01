#include "convert.h"

#define ADC_MAX_RAW  4095u
#define ADC_VREF_MV  3300u

/*
 * Divide n by d (d > 0) and round to the nearest integer,
 * halves away from zero. C division truncates toward zero,
 * so we move n half a step away from zero before dividing.
 */
static int32_t div_round(int32_t n, int32_t d)
{
    if (n >= 0) {
        return (n + d / 2) / d;
    }
    return (n - d / 2) / d;
}

int32_t c10_to_f10(int32_t c10)
{
    /* f10 = c10 * 9 / 5 + 320  ==  (c10 * 9 + 1600) / 5 */
    return div_round(c10 * 9 + 1600, 5);
}

int32_t f10_to_c10(int32_t f10)
{
    /* c10 = (f10 - 320) * 5 / 9 */
    return div_round((f10 - 320) * 5, 9);
}

uint16_t adc_to_mv(uint16_t raw)
{
    uint32_t r = raw;

    if (r > ADC_MAX_RAW) {
        r = ADC_MAX_RAW;
    }

    /* r * 3300 can be ~13.5 million: compute in 32 bits. */
    uint32_t mv = (r * ADC_VREF_MV + ADC_MAX_RAW / 2u) / ADC_MAX_RAW;

    return (uint16_t)mv;
}