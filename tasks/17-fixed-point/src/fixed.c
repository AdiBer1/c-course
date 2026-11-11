
#include "fixed.h"

#include <limits.h>

/*
 * Divide and round to nearest.
 * Halfway cases are rounded away from zero.
 * The denominator must not be zero.
 */
static int64_t round_divide(int64_t numerator, int64_t denominator)
{
    int64_t quotient;
    int64_t remainder;

    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }

    quotient = numerator / denominator;
    remainder = numerator % denominator;

    if ((remainder >= 0 ? remainder : -remainder) * 2 >= denominator) {
        if (numerator >= 0) {
            quotient++;
        } else {
            quotient--;
        }
    }

    return quotient;
}

/* Saturate an int64_t value to the q16_t range. */
static q16_t saturate(int64_t value)
{
    if (value > INT32_MAX) {
        return Q16_MAX;
    }

    if (value < INT32_MIN) {
        return Q16_MIN;
    }

    return (q16_t)value;
}

q16_t q16_from_int(int32_t x)
{
    return saturate((int64_t)x * 65536);
}

int32_t q16_to_int(q16_t x)
{
    return (int32_t)round_divide((int64_t)x, 65536);
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t result;

    result = round_divide((int64_t)milli * 65536, 1000);

    return saturate(result);
}

int32_t q16_to_milli(q16_t x)
{
    return (int32_t)round_divide((int64_t)x * 1000, 65536);
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
    int64_t product;

    product = (int64_t)a * (int64_t)b;

    return saturate(round_divide(product, 65536));
}

q16_t q16_div(q16_t a, q16_t b)
{
    int64_t numerator;

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

    return saturate(round_divide(numerator, (int64_t)b));
}
