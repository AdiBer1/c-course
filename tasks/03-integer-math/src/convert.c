#include "convert.h"

int32_t c10_to_f10(int32_t c10)
{
    int32_t num = c10 * 9 + (c10 >= 0 ? 2 : -2);
    return num / 5 + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    int32_t diff = f10 - 320;
    int32_t num = diff * 5 + (diff >= 0 ? 4 : -4);
    return num / 9;
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095) raw = 4095;
    return (uint16_t)(((uint32_t)raw * 3300 + 2047) / 4095);
}
