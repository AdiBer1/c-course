#include "fixed.h"

/* TODO: implement every function. Write a rounding division helper first. */

q16_t q16_from_int(int32_t x)
{
    int64_t result = (int64_t)x * 65536;

    if (result > Q16_MAX) {
        return Q16_MAX;
    }

    if (result < Q16_MIN) {
        return Q16_MIN;
    }

    return (q16_t)result;
}

int32_t q16_to_int(q16_t x)
{
    int64_t value = x;

    if (value >= 0) {
        return (int32_t)((value + 32768) / 65536);
    }

    return (int32_t)-((-value + 32768) / 65536);
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t numerator = (int64_t)milli * 65536;

    int64_t result;

    if (numerator >= 0) {
        result = (numerator + 500) / 1000;
    } else {
        result = -((-numerator + 500) / 1000);
    }

    if (result > Q16_MAX) {
        return Q16_MAX;
    }

    if (result < Q16_MIN) {
        return Q16_MIN;
    }

    return (q16_t)result;
}

int32_t q16_to_milli(q16_t x)
{
    int64_t numerator = (int64_t)x * 1000;

    if (numerator >= 0) {
        return (int32_t)((numerator + 32768) / 65536);
    }

    return (int32_t)-((-numerator + 32768) / 65536);
}

q16_t q16_add(q16_t a, q16_t b)
{
    int64_t result = (int64_t)a + b;

    if (result > Q16_MAX) {
        return Q16_MAX;
    }

    if (result < Q16_MIN) {
        return Q16_MIN;
    }

    return (q16_t)result;
}

q16_t q16_sub(q16_t a, q16_t b)
{
    int64_t result = (int64_t)a - b;

    if (result > Q16_MAX) {
        return Q16_MAX;
    }

    if (result < Q16_MIN) {
        return Q16_MIN;
    }

    return (q16_t)result;
}

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t product = (int64_t)a * b;
    int64_t result;

    if (product >= 0) {
        result = (product + 32768) / 65536;
    } else {
        result = -((-product + 32768) / 65536);
    }

    if (result > Q16_MAX) {
        return Q16_MAX;
    }

    if (result < Q16_MIN) {
        return Q16_MIN;
    }

    return (q16_t)result;
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
    int64_t denominator = b;

    int negative = ((a < 0) != (b < 0));

    if (numerator < 0) {
        numerator = -numerator;
    }

    if (denominator < 0) {
        denominator = -denominator;
    }

    int64_t quotient = numerator / denominator;
    int64_t remainder = numerator % denominator;

    if (remainder * 2 >= denominator) {
        quotient++;
    }

    if (negative) {
        quotient = -quotient;
    }

    if (quotient > Q16_MAX) {
        return Q16_MAX;
    }

    if (quotient < Q16_MIN) {
        return Q16_MIN;
    }

    return (q16_t)quotient;
}
