#include "outparams.h"

/* TODO: implement every function. */

void swap_i32(int32_t *a, int32_t *b)
{
    if (a == NULL || b == NULL)
    {
        return;
    }

    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

op_status_t min_max_i16(const int16_t *values, size_t len, int16_t *min_out, int16_t *max_out)
{
    if (values == NULL || min_out == NULL || max_out == NULL)
    {
        return OP_ERR_NULL;
    }

    if (len == 0)
    {
        return OP_ERR_EMPTY;
    }

    int16_t min = values[0];
    int16_t max = values[0];

    for (size_t i = 1; i < len; i++)
    {
        if (values[i] < min)
        {
            min = values[i];
        }

        if (values[i] > max)
        {
            max = values[i];
        }
    }

    *min_out = min;
    *max_out = max;

    return OP_OK;
}

op_status_t divmod_u32(uint32_t dividend, uint32_t divisor, uint32_t *quotient, uint32_t *remainder)
{
    if (quotient == NULL || remainder == NULL)
    {
        return OP_ERR_NULL;
    }

    if (divisor == 0)
    {
        return OP_ERR_DIV_ZERO;
    }

    *quotient = dividend / divisor;
    *remainder = dividend % divisor;

    return OP_OK;
}

op_status_t sort3_i32(int32_t *a, int32_t *b, int32_t *c)
{
    if (a == NULL || b == NULL || c == NULL)
    {
        return OP_ERR_NULL;
    }

    int32_t temp;

    if (*a > *b)
    {
        temp = *a;
        *a = *b;
        *b = temp;
    }

    if (*b > *c)
    {
        temp = *b;
        *b = *c;
        *c = temp;
    }

    if (*a > *b)
    {
        temp = *a;
        *a = *b;
        *b = temp;
    }

    return OP_OK;
}
