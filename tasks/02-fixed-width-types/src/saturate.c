#include "saturate.h"

/* TODO: implement every function. Each one currently returns 0. */

uint8_t sat_add_u8(uint8_t a, uint8_t b)
{
    if (UINT8_MAX - a < b) {
        return UINT8_MAX;
    }
    return a + b
    ;
}

uint8_t sat_sub_u8(uint8_t a, uint8_t b)
{
    if (a < b) {
        return 0;
    }
    return a - b;
}

int16_t sat_add_i16(int16_t a, int16_t b)
{
    if (b > 0 && a > INT16_MAX - b) {
        return INT16_MAX;
    }
    if (b < 0 && a < INT16_MIN - b) {
        return INT16_MIN;
    }
    return a + b;
}

int16_t sat_sub_i16(int16_t a, int16_t b)
{
    if (b < 0 && a > INT16_MAX + b) {
        return INT16_MAX;
    }
    if (b > 0 && a < INT16_MIN + b) {
        return INT16_MIN;
    }
    return a - b;
}