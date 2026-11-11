#include "fixed.h"

static int64_t round_div(int64_t numerator, int64_t denominator)
{
    int64_t sign = 1;
    int64_t absolute;
    int64_t quotient;
    int64_t remainder;

    if (numerator < 0) {
        sign = -1;
        absolute = -numerator;
    } else {
        absolute = numerator;
    }

    quotient = absolute / denominator;
    remainder = absolute % denominator;

    if (remainder * 2 >= denominator) {
        quotient++;
    }

    return sign * quotient;
}

static q16_t saturate(int64_t value)
{
    if (value > (int64_t)Q16_MAX) {
        return Q16_MAX;
    }

    if (value < (int64_t)Q16_MIN) {
        return Q16_MIN;
    }

    return (q16_t)value;
}

q16_t q16_from_int(int32_t x)
{
    int64_t raw = (int64_t)x * 65536;

    return saturate(raw);
}

int32_t q16_to_int(q16_t x)
{
    return (int32_t)round_div((int64_t)x, 65536);
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t raw = (int64_t)milli * 65536;

    raw = round_div(raw, 1000);

    return saturate(raw);
}

int32_t q16_to_milli(q16_t x)
{
    int64_t milli = (int64_t)x * 1000;

    return (int32_t)round_div(milli, 65536);
}

q16_t q16_add(q16_t a, q16_t b)
{
    int64_t result = (int64_t)a + (int64_t)b;

    return saturate(result);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    int64_t result = (int64_t)a - (int64_t)b;

    return saturate(result);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t product = (int64_t)a * (int64_t)b;
    int64_t result = round_div(product, 65536);

    return saturate(result);
}

q16_t q16_div(q16_t a, q16_t b)
{
    int64_t numerator;
    int64_t denominator;
    int64_t result;

    if (b == 0) {
        if (a > 0) {
            return Q16_MAX;
        }

        if (a < 0) {
            return Q16_MIN;
        }

        return 0;
    }

    numerator = (int64_t)a * 65536;

    if (b < 0) {
        numerator = -numerator;
        denominator = -(int64_t)b;
    } else {
        denominator = (int64_t)b;
    }

    result = round_div(numerator, denominator);

    return saturate(result);
}