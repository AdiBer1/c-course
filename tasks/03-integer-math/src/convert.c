#include "convert.h"

/*
 * Divide n by d (d > 0) and round to the nearest integer, halves away
 * from zero. C division truncates toward zero, so we first push the
 * numerator half a divisor further from zero: up for positive values,
 * down for negative ones.
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
    /*
     * F = C * 9 / 5 + 32. In tenths: F10 = C10 * 9 / 5 + 320.
     * 320 * 5 = 1600, so F10 = (C10 * 9 + 1600) / 5, rounded once at the end.
     * Multiply before dividing so no precision is lost.
     * Max |C10 * 9 + 1600| = 901600, fits easily in int32_t.
     */
    return div_round(c10 * 9 + 1600, 5);
}

int32_t f10_to_c10(int32_t f10)
{
    /*
     * C = (F - 32) * 5 / 9. In tenths: C10 = (F10 - 320) * 5 / 9.
     * Max |(F10 - 320) * 5| = 501600, fits easily in int32_t.
     */
    return div_round((f10 - 320) * 5, 9);
}

uint16_t adc_to_mv(uint16_t raw)
{
    uint32_t r = raw;

    if (r > 4095u) {
        r = 4095u; /* impossible on real hardware: clamp to full scale */
    }

    /*
     * mV = raw * 3300 / 4095, rounded. 4095 * 3300 = 13513500, which does
     * not fit in 16 bits, so the math is done in uint32_t explicitly
     * (safe even where int is 16 bits). The value is never negative,
     * so adding half the divisor is enough for rounding.
     */
    return (uint16_t)((r * 3300u + 4095u / 2u) / 4095u);
}