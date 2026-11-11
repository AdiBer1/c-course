#include "saturate.h"

uint8_t sat_add_u8(uint8_t a, uint8_t b)
{
    unsigned int sum = (unsigned int)a + (unsigned int)b;
    return sum > UINT8_MAX ? UINT8_MAX : (uint8_t)sum;
}

uint8_t sat_sub_u8(uint8_t a, uint8_t b)
{
    return a < b ? 0 : (uint8_t)(a - b);
}

int16_t sat_add_i16(int16_t a, int16_t b)
{
    int32_t sum = (int32_t)a + (int32_t)b;
    if (sum > INT16_MAX) {
        return INT16_MAX;
    }
    if (sum < INT16_MIN) {
        return INT16_MIN;
    }
    return (int16_t)sum;
}

int16_t sat_sub_i16(int16_t a, int16_t b)
{
    int32_t difference = (int32_t)a - (int32_t)b;
    if (difference > INT16_MAX) {
        return INT16_MAX;
    }
    if (difference < INT16_MIN) {
        return INT16_MIN;
    }
    return (int16_t)difference;
}
