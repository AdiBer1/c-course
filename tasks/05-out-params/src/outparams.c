#include "outparams.h"

void swap_i32(int32_t *a, int32_t *b)
{
    int32_t tmp;

    if (a == NULL || b == NULL) {
        return;
    }
    tmp = *a;
    *a = *b;
    *b = tmp;
}

op_status_t min_max_i16(const int16_t *values, size_t len,
                        int16_t *min_out, int16_t *max_out)
{
    int16_t lo;
    int16_t hi;
    size_t i;

    if (values == NULL || min_out == NULL || max_out == NULL) {
        return OP_ERR_NULL;
    }
    if (len == 0) {
        return OP_ERR_RANGE;
    }
    lo = values[0];
    hi = values[0];
    for (i = 1; i < len; i++) {
        if (values[i] < lo) {
            lo = values[i];
        }
        if (values[i] > hi) {
            hi = values[i];
        }
    }
    *min_out = lo;
    *max_out = hi;
    return OP_OK;
}

op_status_t divmod_u32(uint32_t dividend, uint32_t divisor,
                       uint32_t *quotient, uint32_t *remainder)
{
    if (quotient == NULL || remainder == NULL) {
        return OP_ERR_NULL;
    }
    if (divisor == 0u) {
        return OP_ERR_DIV0;
    }
    *quotient = dividend / divisor;
    *remainder = dividend % divisor;
    return OP_OK;
}

op_status_t sort3_i32(int32_t *a, int32_t *b, int32_t *c)
{
    if (a == NULL || b == NULL || c == NULL) {
        return OP_ERR_NULL;
    }
    if (*a > *b) {
        swap_i32(a, b);
    }
    if (*b > *c) {
        swap_i32(b, c);
    }
    if (*a > *b) {
        swap_i32(a, b);
    }
    return OP_OK;
}