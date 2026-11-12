#include "convert.h"

int32_t c10_to_f10(int32_t c10)
{
    /* F = C * 9 / 5 + 320
     * Rounding to nearest (halves away from zero):
     * For c10 >= 0: (c10 * 9 + 2) / 5 + 320 (or offset before dividing)
     * To compute (c10 * 9 / 5) rounded:
     *   if positive: (c10 * 9 + 2) / 5
     *   if negative: (c10 * 9 - 2) / 5
     */
    int32_t temp = c10 * 9;
    if (temp >= 0) {
        temp = (temp + 2) / 5;
    } else {
        temp = (temp - 2) / 5;
    }
    return temp + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    /* C = (F - 320) * 5 / 9
     * Rounding to nearest (halves away from zero):
     * dividend = (f10 - 320) * 5
     * if dividend >= 0: (dividend + 4) / 9
     * if dividend < 0:  (dividend - 4) / 9
     */
    int32_t diff = f10 - 320;
    int32_t temp = diff * 5;
    if (temp >= 0) {
        return (temp + 4) / 9;
    } else {
        return (temp - 4) / 9;
    }
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095) {
        raw = 4095;
    }
    /* mV = raw * 3300 / 4095
     * Rounding for unsigned values: (raw * 3300 + 4095/2) / 4095
     * 4095 / 2 = 2047
     */
    uint32_t num = (uint32_t)raw * 3300U + 2047U;
    return (uint16_t)(num / 4095U);
}