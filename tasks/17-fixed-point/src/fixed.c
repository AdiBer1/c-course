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
    if (x >= 0) {
        return (int32_t)(((int64_t)x + 0x8000) >> 16);
    } else {
        return (int32_t)(((int64_t)x - 0x8000) >> 16);
    }
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t prod = (int64_t)milli * Q16_ONE;
    int64_t rounded = 0;
    if (prod >= 0) {
        rounded = (prod + 500) / 1000;
    } else {
        rounded = (prod - 500) / 1000;
    }
    if (rounded > INT32_MAX) {
        return Q16_MAX;
    }
    if (rounded < INT32_MIN) {
        return Q16_MIN;
    }
    return (q16_t)rounded;
}

int32_t q16_to_milli(q16_t x)
{
    int64_t prod = (int64_t)x * 1000;
    int64_t rounded = 0;
    if (prod >= 0) {
        rounded = (prod + 32768) / 65536;
    } else {
        rounded = (prod - 32768) / 65536;
    }
    if (rounded > INT32_MAX) {
        return INT32_MAX;
    }
    if (rounded < INT32_MIN) {
        return INT32_MIN;
    }
    return (int32_t)rounded;
}

q16_t q16_add(q16_t a, q16_t b)
{
    int64_t sum = (int64_t)a + (int64_t)b;
    if (sum > INT32_MAX) {
        return Q16_MAX;
    }
    if (sum < INT32_MIN) {
        return Q16_MIN;
    }
    return (q16_t)sum;
}

q16_t q16_sub(q16_t a, q16_t b)
{
    int64_t diff = (int64_t)a - (int64_t)b;
    if (diff > INT32_MAX) {
        return Q16_MAX;
    }
    if (diff < INT32_MIN) {
        return Q16_MIN;
    }
    return (q16_t)diff;
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