#include "convert.h"

int32_t c10_to_f10(int32_t c10)
{
    // C = c10, F_10 = (c10 * 9 / 5) + 320
    // We add/subtract half of the divisor (5 / 2 = 2) for proper rounding.
    int32_t prod = c10 * 9;
    if (prod >= 0) {
        prod += 5 / 2;
    } else {
        prod -= 5 / 2;
    }
    return (prod / 5) + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    // C_10 = (f10 - 320) * 5 / 9
    int32_t diff = f10 - 320;
    int32_t prod = diff * 5;
    if (prod >= 0) {
        prod += 9 / 2;
    } else {
        prod -= 9 / 2;
    }
    return prod / 9;
}

uint16_t adc_to_mv(uint16_t raw)
{
    // Raw values above 4095 must be treated as 4095
    if (raw > 4095) {
        raw = 4095;
    }
    
    // Use uint32_t for multiplication to prevent 16-bit overflow (4095 * 3300 = 13513500)
    uint32_t numerator = (uint32_t)raw * 3300;
    
    // Round to the nearest integer by adding half of the divisor (4095 / 2 = 2047)
    uint32_t rounded = numerator + (4095 / 2);
    
    return (uint16_t)(rounded / 4095);
}