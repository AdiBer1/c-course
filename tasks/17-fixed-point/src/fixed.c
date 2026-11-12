#include "fixed.h"

#define Q16_SCALE 65536LL
#define MILLI_SCALE 1000LL

static q16_t saturate_q16(int64_t val)
{
    if (val > (int64_t)Q16_MAX) {
        return Q16_MAX;
    }
    if (val < (int64_t)Q16_MIN) {
        return Q16_MIN;
    }
    return (q16_t)val;
}

static int64_t div_round_away(int64_t num, int64_t den)
{
    if (den < 0) {
        num = -num;
        den = -den;
    }
    if (num >= 0) {
        return (num + (den / 2)) / den;
    }
    return (num - (den / 2)) / den;
}

q16_t q16_from_int(int32_t x)
{
    return saturate_q16((int64_t)x * Q16_SCALE);
}

int32_t q16_to_int(q16_t x)
{
    return (int32_t)div_round_away((int64_t)x, Q16_SCALE);
}

q16_t q16_from_milli(int32_t milli)
{
    return saturate_q16(div_round_away((int64_t)milli * Q16_SCALE, MILLI_SCALE));
}

int32_t q16_to_milli(q16_t x)
{
    return (int32_t)div_round_away((int64_t)x * MILLI_SCALE, Q16_SCALE);
}

q16_t q16_add(q16_t a, q16_t b)
{
    return saturate_q16((int64_t)a + (int64_t)b);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    return saturate_q16((int64_t)a - (int64_t)b);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    return saturate_q16(div_round_away((int64_t)a * (int64_t)b, Q16_SCALE));
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
    return saturate_q16(div_round_away((int64_t)a * Q16_SCALE, (int64_t)b));
}