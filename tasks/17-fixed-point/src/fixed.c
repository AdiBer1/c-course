#include "fixed.h"

#define Q16_SCALE ((int64_t)1 << Q16_FRAC_BITS) /* 65536 */

/*
 * n / d rounded to the nearest integer, halves away from zero. d must be > 0.
 * C division truncates toward zero, so we divide the MAGNITUDE of n with
 * "+ half the divisor" (which rounds a positive quotient correctly) and then
 * put the sign back. The callers keep |n| below 2^63 - 2^31, so the
 * addition here cannot overflow.
 */
static int64_t round_div(int64_t n, int64_t d)
{
    if (n < 0) {
        return -((-n + d / 2) / d);
    }
    return (n + d / 2) / d;
}

/* Clamp a 64-bit result into the Q16.16 range: SATURATE, never wrap. */
static q16_t saturate(int64_t v)
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
    /* x * 65536 instead of x << 16: shifting a negative number left is
     * undefined behaviour, and 64 bits mean the product cannot overflow. */
    return saturate((int64_t)x * Q16_SCALE);
}

int32_t q16_to_int(q16_t x)
{
    /* |result| <= 32768, always fits in int32_t */
    return (int32_t)round_div((int64_t)x, Q16_SCALE);
}

q16_t q16_from_milli(int32_t milli)
{
    /* value = milli / 1000, raw = value * 65536 = milli * 65536 / 1000 */
    return saturate(round_div((int64_t)milli * Q16_SCALE, 1000));
}

int32_t q16_to_milli(q16_t x)
{
    /* milli = raw * 1000 / 65536; |result| <= 32768000, fits in int32_t */
    return (int32_t)round_div((int64_t)x * 1000, Q16_SCALE);
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
    /*
     * a = A * 65536, b = B * 65536, so a * b = A * B * 65536^2.
     * One division by 65536 leaves A * B * 65536: the Q16.16 result.
     * The product needs up to 62 bits, so it is computed in int64_t.
     */
    return saturate(round_div((int64_t)a * (int64_t)b, Q16_SCALE));
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
     * (A * 65536) / (B * 65536) = A / B would lose the fraction, so the
     * dividend is scaled up by 65536 first: (a * 65536) / b = A / B * 65536.
     * a * 65536 needs up to 48 bits: int64_t.
     */
    int64_t n = (int64_t)a * Q16_SCALE;
    int64_t d = (int64_t)b;
    if (d < 0) { /* round_div needs a positive divisor: move the sign to n */
        n = -n;
        d = -d;
    }
    return saturate(round_div(n, d));
}