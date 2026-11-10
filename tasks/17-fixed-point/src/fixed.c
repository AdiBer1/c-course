#include "fixed.h"

static int64_t div_round64(int64_t num, int64_t den)
{
    if (den == 0) {
        return 0;
    }
    if (num >= 0) {
        if (den > 0) {
            return (num + (den / 2)) / den;
        } else {
            return (num - (den / 2)) / den;
        }
    } else {
        if (den > 0) {
            return (num - (den / 2)) / den;
        } else {
            return (num + (den / 2)) / den;
        }
    }
}

static q16_t clamp64_to_q16(int64_t val)
{
    if (val > (int64_t)Q16_MAX) {
        return Q16_MAX;
    }
    if (val < (int64_t)Q16_MIN) {
        return Q16_MIN;
    }
    return (q16_t)val;
}

q16_t q16_from_int(int32_t x)
{
    int64_t val = (int64_t)x * 65536LL;
    return clamp64_to_q16(val);
}

int32_t q16_to_int(q16_t x)
{
    int64_t res = div_round64((int64_t)x, 65536LL);
    if (res > (int64_t)INT32_MAX) {
        return INT32_MAX;
    }
    if (res < (int64_t)INT32_MIN) {
        return INT32_MIN;
    }
    return (int32_t)res;
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t num = (int64_t)milli * 65536LL;
    int64_t res = div_round64(num, 1000LL);
    return clamp64_to_q16(res);
}

int32_t q16_to_milli(q16_t x)
{
    int64_t num = (int64_t)x * 1000LL;
    int64_t res = div_round64(num, 65536LL);
    if (res > (int64_t)INT32_MAX) {
        return INT32_MAX;
    }
    if (res < (int64_t)INT32_MIN) {
        return INT32_MIN;
    }
    return (int32_t)res;
}

q16_t q16_add(q16_t a, q16_t b)
{
    int64_t sum = (int64_t)a + (int64_t)b;
    return clamp64_to_q16(sum);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    int64_t diff = (int64_t)a - (int64_t)b;
    return clamp64_to_q16(diff);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t prod = (int64_t)a * (int64_t)b;
    int64_t res = div_round64(prod, 65536LL);
    return clamp64_to_q16(res);
}

q16_t q16_div(q16_t a, q16_t b)
{
    if (b == 0) {
        if (a > 0) return Q16_MAX;
        if (a < 0) return Q16_MIN;
        return 0;
    }

    int64_t num = (int64_t)a * 65536LL;
    int64_t res = div_round64(num, (int64_t)b);
    return clamp64_to_q16(res);
}