#include <stdint.h>
#include "saturate.h"

uint8_t sat_add_u8(uint8_t a, uint8_t b)
{
    return (b > UINT8_MAX - a) ? UINT8_MAX : (uint8_t)(a + b);
}

uint8_t sat_sub_u8(uint8_t a, uint8_t b)
{
    return (b > a) ? 0 : (uint8_t)(a - b);
}

int16_t sat_add_i16(int16_t a, int16_t b)
{
    int32_t result = (int32_t)a + (int32_t)b;
    if (result > INT16_MAX) return INT16_MAX;
    if (result < INT16_MIN) return INT16_MIN;
    return (int16_t)result;
}

int16_t sat_sub_i16(int16_t a, int16_t b)
{
    int32_t result = (int32_t)a - (int32_t)b;
    if (result > INT16_MAX) return INT16_MAX;
    if (result < INT16_MIN) return INT16_MIN;
    return (int16_t)result;
}
