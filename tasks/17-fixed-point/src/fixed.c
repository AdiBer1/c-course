#include "fixed.h"

/*
 * Integer division rounded to the nearest result, halves away from zero.
 * C division truncates toward zero, so the quotient is fixed using the
 * remainder: if |r| is at least half of |d|, move one step away from zero.
 *   7 / 2  -> q = 3,  r = 1  -> 2*1 >= 2 -> 4
 *  -7 / 2  -> q = -3, r = -1 -> 2*1 >= 2 -> -4
 * Precondition: d != 0.
 */
static int64_t div_round(int64_t n, int64_t d)
{
    int64_t q = n / d;
    int64_t r = n % d;
    int64_t abs_r = (r < 0) ? -r : r;
    int64_t abs_d = (d < 0) ? -d : d;

    if (2 * abs_r >= abs_d) {
        if ((n < 0) == (d < 0)) {
            q += 1; /* positive result: round up */
        } else {
            q -= 1; /* negative result: round down (away from zero) */
        }
    }
    return q;
}

/* Saturate a 64-bit value into the q16_t range. */
static q16_t clamp_q16(int64_t v)
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
    /* multiply instead of << : shifting a negative value is undefined */
    return clamp_q16((int64_t)x * Q16_ONE);
}

int32_t q16_to_int(q16_t x)
{
    /* result is at most 32768 in magnitude, so it always fits */
    return (int32_t)div_round((int64_t)x, Q16_ONE);
}

q16_t q16_from_milli(int32_t milli)
{
    return clamp_q16(div_round((int64_t)milli * Q16_ONE, 1000));
}

int32_t q16_to_milli(q16_t x)
{
    /* at most 32768000 in magnitude, fits in int32_t */
    return (int32_t)div_round((int64_t)x * 1000, Q16_ONE);
}

q16_t q16_add(q16_t a, q16_t b)
{
    return clamp_q16((int64_t)a + (int64_t)b);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    return clamp_q16((int64_t)a - (int64_t)b);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    /* (A*65536) * (B*65536) = A*B*65536^2 -> divide once by 65536 */
    int64_t product = (int64_t)a * (int64_t)b;
    return clamp_q16(div_round(product, Q16_ONE));
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
    /*
     * (A*65536) / (B*65536) = A/B with no scale left, so scale the
     * numerator by 65536 first. Q16_MIN / -1 gives +2^31, which the
     * clamp turns into Q16_MAX.
     */
    return clamp_q16(div_round((int64_t)a * Q16_ONE, (int64_t)b));
}
