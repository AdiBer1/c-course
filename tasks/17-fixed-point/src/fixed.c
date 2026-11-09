#include "fixed.h"

static q16_t sat(int64_t x)
{
    if (x > INT32_MAX) return Q16_MAX;
    if (x < INT32_MIN) return Q16_MIN;
    return (q16_t)x;
}

/* Round to nearest, halves away from zero */
static int64_t round_div(int64_t num, int64_t den)
{
    int64_t q = num / den;
    int64_t r = num % den;
    /* if remainder * 2 >= den (in absolute terms), round away from zero */
    if (r < 0) r = -r;
    if (den < 0) den = -den;
    if (r * 2 >= den) {
        if ((num ^ (num / den)) >= 0)
            q++;
        else
            q--;
    }
    return q;
}

q16_t q16_from_int(int32_t x)
{
    int64_t v = (int64_t)x * 65536;
    return sat(v);
}

int32_t q16_to_int(q16_t x)
{
    int64_t num = (int64_t)x;
    int64_t q   = num / 65536;
    int64_t r   = num % 65536;
    if (r < 0) r = -r;
    if (r * 2 >= 65536) {
        if (x >= 0) q++;
        else        q--;
    }
    return (int32_t)q;
}

q16_t q16_from_milli(int32_t milli)
{
    /* milli/1000 in Q16.16 = milli * 65536 / 1000 */
    int64_t num = (int64_t)milli * 65536;
    int64_t result = round_div(num, 1000);
    return sat(result);
}

int32_t q16_to_milli(q16_t x)
{
    /* x * 1000 / 65536, rounded */
    int64_t num = (int64_t)x * 1000;
    return (int32_t)round_div(num, 65536);
}

q16_t q16_add(q16_t a, q16_t b)
{
    return sat((int64_t)a + b);
}

q16_t q16_sub(q16_t a, q16_t b)
{
    return sat((int64_t)a - b);
}

q16_t q16_mul(q16_t a, q16_t b)
{
    int64_t prod = (int64_t)a * b;
    int64_t result = round_div(prod, 65536);
    return sat(result);
}

q16_t q16_div(q16_t a, q16_t b)
{
    if (b == 0) {
        if (a > 0) return Q16_MAX;
        if (a < 0) return Q16_MIN;
        return 0;
    }
    int64_t num    = (int64_t)a * 65536;
    int64_t result = round_div(num, (int64_t)b);
    return sat(result);
}
