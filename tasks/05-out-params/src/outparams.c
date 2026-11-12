#include "outparams.h"

void swap_i32(int32_t *a, int32_t *b)
{
    if (a == NULL || b == NULL) {
        return;
    }
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

op_status_t min_max_i16(const int16_t *values, size_t len, int16_t *min_out, int16_t *max_out)
{
    if (values == NULL || min_out == NULL || max_out == NULL) {
        return OP_ERR_NULL;
    }
    if (len == 0) {
        return OP_ERR_EMPTY;
    }

    int16_t min_val = values[0];
    int16_t max_val = values[0];

    for (size_t i = 1; i < len; i++) {
        if (values[i] < min_val) {
            min_val = values[i];
        }
        if (values[i] > max_val) {
            max_val = values[i];
        }
    }

    *min_out = min_val;
    *max_out = max_val;
    return OP_OK;
}

op_status_t divmod_u32(uint32_t dividend, uint32_t divisor, uint32_t *quotient,
                       uint32_t *remainder)
{
    if (quotient == NULL || remainder == NULL) {
        return OP_ERR_NULL;
    }
    if (divisor == 0) {
        return OP_ERR_DIV_ZERO;
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

    int32_t val_a = *a;
    int32_t val_b = *b;
    int32_t val_c = *c;

    if (val_a > val_b) {
        int32_t tmp = val_a;
        val_a = val_b;
        val_b = tmp;
    }
    if (val_b > val_c) {
        int32_t tmp = val_b;
        val_b = val_c;
        val_c = tmp;
    }
    if (val_a > val_b) {
        int32_t tmp = val_a;
        val_a = val_b;
        val_b = tmp;
    }

    *a = val_a;
    *b = val_b;
    *c = val_c;

    return OP_OK;
}