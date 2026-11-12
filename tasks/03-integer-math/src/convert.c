#include "convert.h"

/* TODO: implement every function. Each one currently returns 0. */

int32_t c10_to_f10(int32_t c10)
{
    int32_t num = c10 * 9 + 1600;
    if (num >= 0) {
        return (num + 2) / 5;
    } else {
        return (num - 2) / 5;
    }
}

int32_t f10_to_c10(int32_t f10)
{
    int32_t num = f10 * 5 - 1600;
    if (num >= 0) {
        return (num + 4) / 9;
    } else {
        return (num - 4) / 9;
    }
}


uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095) {
        raw = 4095;
    }
    uint32_t num = (uint32_t)raw * 3300 + 2047;
    return (uint16_t)(num / 4095);
}
