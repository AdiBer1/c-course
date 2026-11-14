#include "fixed.h"

q16_t q16_from_int(int32_t x)
{
    if (x > 32767) {
        return Q16_MAX;
    }
    if (x < -32768) {
        return Q16_MIN;
    }
    return (q16_t)((int64_t)x * Q16_ONE);
}

int32_t q16_to_int(q16_t x)
{
    int64_t val = (int64_t)x;
    int64_t sign = 1;
    if (val < 0) {
        sign = -1;
        val = -val;
    }
    val = (val + (Q16_ONE / 2)) >> Q16_FRAC_BITS;
    val *= sign;

    if (val > INT32_MAX) {
        return INT32_MAX;
    }
    if (val < INT32_MIN) {
        return INT32_MIN;
    }
    return (int32_t)val;
}

q16_t q16_from_milli(int32_t milli)
{
    int64_t product = (int64_t)milli * Q16_ONE;  
    int64_t rounded;
    
    if (product >= 0) {
        rounded = (product + 500) / 1000;
    } else {
        rounded = (product - 500) / 1000;
    }

    if (rounded > INT32_MAX) {
        return Q16_MAX;
    }
    if (rounded < INT32_MIN) {
        return Q16_MIN;
    }

    return (q16_t)rounded;
}

int32_t q16_to_milli(q16_t x)
{
    int64_t product = (int64_t)x * 1000;
    int64_t rounded;

    if (product >= 0) {
        rounded = (product + (Q16_ONE / 2)) / Q16_ONE;
    } else {
        rounded = (product - (Q16_ONE / 2)) / Q16_ONE;
    }
    if (rounded > INT32_MAX) {
        return INT32_MAX;
    }
    if (rounded < INT32_MIN) {
        return INT32_MIN;
    }

    return (int32_t)rounded;
}

q16_t q16_add(q16_t a, q16_t b)
{
    int64_t sum = (int64_t)a + b;
    if (sum > INT32_MAX) {
        return Q16_MAX;
    }
    if (sum < INT32_MIN) {
        return Q16_MIN;
    }
    return (q16_t)sum;
}

q16_t q16_sub(q16_t a, q16_t b)
{
    int64_t diff = (int64_t)a - b;
    if (diff > INT32_MAX) {
        return Q16_MAX;
    }
    if (diff < INT32_MIN) {
        return Q16_MIN;
    }
    return (q16_t)diff;
}

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t product = (int64_t)a * b;
    int64_t sign = 1;
    if (product < 0) {
        sign = -1;
        product = -product;
    }
    product += (Q16_ONE / 2);
    int64_t res = product >> Q16_FRAC_BITS;
    res *= sign;

    if (res > INT32_MAX) {
        return Q16_MAX;
    }
    if (res < INT32_MIN) {
        return Q16_MIN;
    }

    return (q16_t)res;
}

q16_t q16_div(q16_t a, q16_t b)
{
    if (b == 0) {
        if (a > 0) return Q16_MAX;
        if (a < 0) return Q16_MIN;
        return 0;
    }

    int64_t numerator = (int64_t)a * Q16_ONE;
    
    int64_t quotient;
    if ((numerator >= 0 && b > 0) || (numerator < 0 && b < 0)) {
        quotient = (numerator + (b / 2)) / b;
    } else {
        quotient = (numerator - (b / 2)) / b;
    }

    if (quotient > INT32_MAX) {
        return Q16_MAX;
    }
    if (quotient < INT32_MIN) {
        return Q16_MIN;
    }

    return (q16_t)quotient;
}