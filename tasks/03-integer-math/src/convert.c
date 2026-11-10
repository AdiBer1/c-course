#include "convert.h"

int32_t c10_to_f10(int32_t c10)
{
    int32_t n = c10 * 9;
    int32_t q;

    if (n >= 0) {
        q = (n + 2) / 5;
    } else {
        q = (n - 2) / 5;
    }
    return q + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    int32_t n = (f10 - 320) * 5;

    if (n >= 0) {
        return (n + 4) / 9;
    }
    return (n - 4) / 9;
}

uint16_t adc_to_mv(uint16_t raw)
{
    uint32_t n = (uint32_t)raw * 3300u;

    return (uint16_t)((n + 2047u) / 4095u);
}