#include "fixed.h"

#include <stdint.h>

/* TODO: implement every function. Write a rounding division helper first. */

q16_t q16_from_int(int32_t x)
{
    if (x < -32768) {
        return Q16_MIN;
    }
    if (x > 32767) {
        return Q16_MAX;
    }
    return x * Q16_ONE;
}

int32_t q16_to_int(q16_t x)
{
    int32_t int_part = x >> 16;
    int32_t frac_part = x & 0xFFFF;
    
    if (x >= 0) {
        if (frac_part >= 0x8000) {
            int_part++;
        }
    } else {
        if (frac_part > 0x8000 || (frac_part == 0x8000 && (int_part & 1) != 0)) {
            // Rounds halves away from zero
            // For negative numbers, away from zero means more negative (subtract 1 from magnitude, or add 1 depending on representation)
        }
    }
    
    // Simpler correct round to nearest, halves away from zero for all 32-bit:
    int64_t rounded = ((int64_t)x + 0x8000) >> 16;
    if (x < 0 && (x & 0xFFFF) == 0x8000) {
        rounded = (x >> 16); // exact half away from zero for negative
    }
    
    // Let's write a robust rounding conversion:
    int32_t res = 0;
    if (x >= 0) {
        res = (int32_t)(((int64_t)x + 0x8000) >> 16);
    } else {
        res = (int32_t)(((int64_t)x - 0x8000) >> 16);
    }
    return res;
}

q16_t q16_from_milli(int32_t milli)
{
    (void)milli;
    return 0;
}

int32_t q16_to_milli(q16_t x)
{
    (void)x;
    return 0;
}

q16_t q16_add(q16_t a, q16_t b)
{
    (void)a;
    (void)b;
    return 0;
}

q16_t q16_sub(q16_t a, q16_t b)
{
    (void)a;
    (void)b;
    return 0;
}

q16_t q16_mul(q16_t a, q16_t b)
{
    (void)a;
    (void)b;
    return 0;
}

q16_t q16_div(q16_t a, q16_t b)
{
    (void)a;
    (void)b;
    return 0;
}