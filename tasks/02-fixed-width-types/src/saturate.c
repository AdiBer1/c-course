#include "saturate.h"

/*
 * Strategy: do the arithmetic in a wider type where the exact result always
 * fits, then clamp it back into the target range. Because the wide result is
 * exact, no step can overflow, so there is no undefined behaviour.
 */

uint8_t sat_add_u8(uint8_t a, uint8_t b)
{
    /* Max possible sum is 255 + 255 = 510, fits easily in uint16_t. */
    uint16_t sum = (uint16_t)((uint16_t)a + (uint16_t)b);

    if (sum > UINT8_MAX) {
        return UINT8_MAX;
    }
    return (uint8_t)sum;
}

uint8_t sat_sub_u8(uint8_t a, uint8_t b)
{
    /* For unsigned values the only way out of range is going below 0. */
    if (a <= b) {
        return 0U;
    }
    return (uint8_t)(a - b);
}

int16_t sat_add_i16(int16_t a, int16_t b)
{
    /* Range of a + b is [-65536, 65534]: always fits in int32_t. */
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
    /* Range of a - b is [-65535, 65535]: always fits in int32_t.
     * This also covers 0 - INT16_MIN, whose result (32768) does not fit
     * in int16_t but is fine in int32_t before clamping. */
    int32_t diff = (int32_t)a - (int32_t)b;

    if (diff > INT16_MAX) {
        return INT16_MAX;
    }
    if (diff < INT16_MIN) {
        return INT16_MIN;
    }
    return (int16_t)diff;
}
