
#include "fixed.h"

static int64_t round_div(int64_t numerator, int64_t denominator)
{
    int64_t quotient = numerator / denominator;
    int64_t remainder = numerator % denominator;

    if (remainder != 0) {
        int64_t rem_abs = (remainder < 0) ? -remainder : remainder;
        int64_t den_abs = (denominator < 0) ? -denominator : denominator;

        if (rem_abs * 2 >= den_abs) {
            if ((numerator < 0) != (denominator < 0)) {
                quotient--;
            } else {
                quotient++;
            }
        }
    }

    return quotient;
}

static q16_t clamp_q16(int64_t value)
{
    if (value > Q16_MAX) {
        return Q16_MAX;
    }

    if (value < Q16_MIN) {
        return Q16_MIN;
    }

    return (q16_t)value;
}

q16_t q16_from_int(int32_t x)
{
    int64_t result = (int64_t)x * 65536;
    return clamp_q16(result);
}

int32_t q16_to_int(q16_t x)
{
    return (int32_t)round_div(x, 65536);
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t numerator = (int64_t)milli * 65536;
    return clamp_q16(round_div(numerator, 1000));
}

int32_t q16_to_milli(q16_t x)
{
    int64_t numerator = (int64_t)x * 1000;
    return (int32_t)round_div(numerator, 65536);
}

q16_t q16_add(q16_t a, q16_t b)
{
    int64_t result = (int64_t)a + b;
    return clamp_q16(result);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    int64_t result = (int64_t)a - b;
    return clamp_q16(result);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t product = (int64_t)a * b;
    int64_t result = round_div(product, 65536);
    return clamp_q16(result);
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
    int64_t result = round_div(numerator, b);

    return clamp_q16(result);
}
