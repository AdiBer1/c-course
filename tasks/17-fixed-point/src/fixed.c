#include "fixed.h"

#include <stdint.h>

/* TODO: implement every function. Write a rounding division helper first. */

static int64_t div_round(int64_t num, int64_t den)
{
    if ((num >= 0 && den > 0) || (num < 0 && den < 0)) {
        return (num + (den / 2)) / den;
    }
    return (num - (den / 2)) / den;
}

static q16_t clamp_q16(int64_t val)
{
    if (val > INT32_MAX) {
        return Q16_MAX;
    }
    if (val < INT32_MIN) {
        return Q16_MIN;
    }
    return (q16_t)val;
}

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
    return (int32_t)div_round((int64_t)x, 65536);
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t prod = (int64_t)milli * 65536;
    return clamp_q16(div_round(prod, 1000));
}

int32_t q16_to_milli(q16_t x)
{
    int64_t prod = (int64_t)x * 1000;
    int64_t rounded = div_round(prod, 65536);
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
    return clamp_q16((int64_t)a + (int64_t)b);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    return clamp_q16((int64_t)a - (int64_t)b);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t prod = (int64_t)a * (int64_t)b;
    return clamp_q16(div_round(prod, 65536));
}

q16_t q16_div(q16_t a, q16_t b)
{
    if (b == 0) {
        if (a > 0) {
            return Q16_MAX;
        }
        if (a < 0) {
            return Q16_MIN;
        }
        return 0;
    }

    int64_t numerator = (int64_t)a * 65536;
    return clamp_q16(div_round(numerator, (int64_t)b));
}