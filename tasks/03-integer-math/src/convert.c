#include "convert.h"

int32_t c10_to_f10(int32_t c10)
{
    int32_t numerator = 18 * c10 + 3200;
    if (numerator >= 0) {
        return (numerator + 5) / 10;
    } else {
        return (numerator - 5) / 10;
    }
}

int32_t f10_to_c10(int32_t f10)
{
    int32_t numerator = 5 * f10 - 1600;
    
    if (numerator >= 0) {
        return (numerator + 4) / 9;
    } else {
        return (numerator - 4) / 9;
    }
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095) {
        raw = 4095;
    }
    uint32_t numerator = (uint32_t)raw * 3300;
    return (uint16_t)((numerator + 2047) / 4095);
}