#include "outparams.h"

/*
 * Pattern used by every function here:
 *   1. check every pointer for NULL
 *   2. check the other error conditions
 *   3. compute into local variables
 *   4. only then write the outputs and return OP_OK
 * So on any error the caller's variables are never touched.
 */

void swap_i32(int32_t *a, int32_t *b)
{
    if (a == NULL || b == NULL) {
        return;
    }

    int32_t tmp = *a; /* the "third cup" */
    *a = *b;
    *b = tmp;
}

op_status_t min_max_i16(const int16_t *values, size_t len, int16_t *min_out, int16_t *max_out)
{
    if (values == NULL || min_out == NULL || max_out == NULL) {
        return OP_ERR_NULL;
    }
    if (len == 0u) {
        return OP_ERR_EMPTY;
    }

    /* Start from the first element, not from 0: {-5, -9} must give max -5. */
    int16_t min = values[0];
    int16_t max = values[0];

    for (size_t i = 1u; i < len; i++) {
        if (values[i] < min) {
            min = values[i];
        }
        if (values[i] > max) {
            max = values[i];
        }
    }

    *min_out = min;
    *max_out = max;
    return OP_OK;
}

op_status_t divmod_u32(uint32_t dividend, uint32_t divisor, uint32_t *quotient, uint32_t *remainder)
{
    if (quotient == NULL || remainder == NULL) {
        return OP_ERR_NULL;
    }
    if (divisor == 0u) {
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

    /* Sort local copies with three compare-and-swap steps. */
    int32_t x = *a;
    int32_t y = *b;
    int32_t z = *c;

    if (x > y) {
        swap_i32(&x, &y);
    }
    if (y > z) {
        swap_i32(&y, &z);
    }
    if (x > y) {
        swap_i32(&x, &y);
    }

    *a = x;
    *b = y;
    *c = z;
    return OP_OK;
}