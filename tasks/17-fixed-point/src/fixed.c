#include "fixed.h"

#include <stdint.h>

/* n / d rounded to nearest, halves away from zero. d must be > 0. */
static int64_t round_div(int64_t n, int64_t d)
{
    if (n >= 0) {
        return (n + d / 2) / d;
    }
    return -((-n + d / 2) / d);
}

/* Clamp a 64-bit raw value into the Q16.16 range. */
static q16_t saturate(int64_t v)
{
    if (v > Q16_MAX) {
        return Q16_MAX;
    }
    if (v < Q16_MIN) {
        return Q16_MIN;
    }
    return (q16_t)v;
}

q16_t q16_from_int(int32_t x)
{
    if (x > 32767) {
        return Q16_MAX;
    }
    if (x < -32768) {
        return Q16_MIN;
    }
    return (q16_t)((int64_t)x * 65536);
}

int32_t q16_to_int(q16_t x)
{
    return (int32_t)round_div(x, 65536);
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t num = (int64_t)milli * 65536;
    return saturate(round_div(num, 1000));
}

int32_t q16_to_milli(q16_t x)
{
    return (int32_t)round_div((int64_t)x * 1000, 65536);
}

q16_t q16_add(q16_t a, q16_t b)
{
    return saturate((int64_t)a + b);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    return saturate((int64_t)a - b);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    return saturate(round_div((int64_t)a * b, 65536));
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

    int64_t n = (int64_t)a * 65536;
    int64_t d = b;
    if (d < 0) {
        n = -n;
        d = -d;
    }
    return saturate(round_div(n, d));
}