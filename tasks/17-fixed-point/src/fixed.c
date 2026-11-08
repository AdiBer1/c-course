#include "fixed.h"

#define Q16_SCALE 65536

/* Divide num by den (den > 0), rounding to nearest, halves away from zero. */
static int64_t div_round(int64_t num, int64_t den)
{
    if (num >= 0) {
        return (num + den / 2) / den;
    }
    return -((-num + den / 2) / den);
}

/* Clamp a 64-bit value into the q16_t range. */
static q16_t saturate(int64_t v)
{
    if (v > INT32_MAX) {
        return Q16_MAX;
    }
    if (v < INT32_MIN) {
        return Q16_MIN;
    }
    return (q16_t)v;
}

q16_t q16_from_int(int32_t x)
{
    return saturate((int64_t)x * Q16_SCALE);
}

int32_t q16_to_int(q16_t x)
{
    return (int32_t)div_round((int64_t)x, Q16_SCALE);
}

q16_t q16_from_milli(int32_t milli)
{
    return saturate(div_round((int64_t)milli * Q16_SCALE, 1000));
}

int32_t q16_to_milli(q16_t x)
{
    return (int32_t)div_round((int64_t)x * 1000, Q16_SCALE);
}

q16_t q16_add(q16_t a, q16_t b)
{
    return saturate((int64_t)a + (int64_t)b);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    return saturate((int64_t)a - (int64_t)b);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    /* the raw product has 32 fraction bits: scale back down to 16 */
    return saturate(div_round((int64_t)a * (int64_t)b, Q16_SCALE));
}

q16_t q16_div(q16_t a, q16_t b)
{
    int64_t num;
    int64_t den;

    if (b == 0) {
        if (a > 0) {
            return Q16_MAX;
        }
        if (a < 0) {
            return Q16_MIN;
        }
        return 0;
    }

    /* scale the numerator first so the result keeps 16 fraction bits */
    num = (int64_t)a * Q16_SCALE;
    den = (int64_t)b;
    if (den < 0) {
        num = -num;
        den = -den;
    }
    return saturate(div_round(num, den));
}