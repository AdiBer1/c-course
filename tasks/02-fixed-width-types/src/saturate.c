#include "saturate.h"

/*
 * Every function computes the exact result in a wider type (int32_t),
 * where it always fits, compares it with the limits of the narrow type,
 * and only then converts it back. Nothing can overflow along the way.
 */

uint8_t sat_add_u8(uint8_t a, uint8_t b)
{
    int32_t wide = (int32_t)a + (int32_t)b; /* 0 .. 510, always fits */

    if (wide > UINT8_MAX) {
        return UINT8_MAX;
    }
    return (uint8_t)wide;
}

uint8_t sat_sub_u8(uint8_t a, uint8_t b)
{
    int32_t wide = (int32_t)a - (int32_t)b; /* -255 .. 255, always fits */

    if (wide < 0) {
        return 0;
    }
    return (uint8_t)wide;
}

int16_t sat_add_i16(int16_t a, int16_t b)
{
    int32_t wide = (int32_t)a + (int32_t)b; /* -65536 .. 65534, always fits */

    if (wide > INT16_MAX) {
        return INT16_MAX;
    }
    if (wide < INT16_MIN) {
        return INT16_MIN;
    }
    return (int16_t)wide;
}

int16_t sat_sub_i16(int16_t a, int16_t b)
{
    int32_t wide = (int32_t)a - (int32_t)b; /* -65535 .. 65535, always fits */

    if (wide > INT16_MAX) {
        return INT16_MAX;
    }
    if (wide < INT16_MIN) {
        return INT16_MIN;
    }
    return (int16_t)wide;
}