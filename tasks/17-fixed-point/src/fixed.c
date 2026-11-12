#include "fixed.h"

static int64_t div_round(int64_t num, int64_t den)
{
    int64_t abs_num = (num < 0) ? -num : num;
    int64_t q = (abs_num + den / 2) / den;

    return (num < 0) ? -q : q;
}

static q16_t saturate(int64_t v)
{
    if (v > (int64_t)Q16_MAX) {
        return Q16_MAX;
    }
    if (v < (int64_t)Q16_MIN) {
        return Q16_MIN;
    }
    return (q16_t)v;
}

q16_t q16_from_int(int32_t x)
{
    /* multiply instead of x << 16: shifting a negative number is undefined */
    return saturate((int64_t)x * Q16_ONE);
}

int32_t q16_to_int(q16_t x)
{
    return (int32_t)div_round((int64_t)x, (int64_t)Q16_ONE);
}

q16_t q16_from_milli(int32_t milli)
{
    return saturate(div_round((int64_t)milli * Q16_ONE, 1000));
}

int32_t q16_to_milli(q16_t x)
{
    return (int32_t)div_round((int64_t)x * 1000, (int64_t)Q16_ONE);
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
    return saturate(div_round((int64_t)a * (int64_t)b, (int64_t)Q16_ONE));
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

    num = (int64_t)a * Q16_ONE;
    den = (int64_t)b;

    if (den < 0) {
        num = -num;
        den = -den;
    }

    return saturate(div_round(num, den));
}