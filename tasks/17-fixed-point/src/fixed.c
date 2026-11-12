#include "fixed.h"

static q16_t saturate_i64(int64_t val)
{
    if (val > (int64_t)Q16_MAX) {
        return Q16_MAX;
    }
    if (val < (int64_t)Q16_MIN) {
        return Q16_MIN;
    }
    return (q16_t)val;
}

static int64_t div_round_nearest(int64_t num, int64_t den)
{
    if (den < 0) {
        num = -num;
        den = -den;
    }
    int64_t half = den / 2;
    if (num >= 0) {
        return (num + half) / den;
    }
    return (num - half) / den;
}

q16_t q16_from_int(int32_t x)
{
    int64_t scaled = (int64_t)x * (int64_t)Q16_ONE;
    return saturate_i64(scaled);
}

int32_t q16_to_int(q16_t x)
{
    return (int32_t)div_round_nearest((int64_t)x, (int64_t)Q16_ONE);
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t num = (int64_t)milli * (int64_t)Q16_ONE;
    int64_t res = div_round_nearest(num, 1000);
    return saturate_i64(res);
}

int32_t q16_to_milli(q16_t x)
{
    int64_t num = (int64_t)x * 1000;
    return (int32_t)div_round_nearest(num, (int64_t)Q16_ONE);
}

q16_t q16_add(q16_t a, q16_t b)
{
    int64_t sum = (int64_t)a + (int64_t)b;
    return saturate_i64(sum);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    int64_t diff = (int64_t)a - (int64_t)b;
    return saturate_i64(diff);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t prod = (int64_t)a * (int64_t)b;
    int64_t res = div_round_nearest(prod, (int64_t)Q16_ONE);
    return saturate_i64(res);
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
    int64_t num = (int64_t)a * (int64_t)Q16_ONE;
    int64_t res = div_round_nearest(num, (int64_t)b);
    return saturate_i64(res);
}