#include "ops.h"

/* ---------------------------------------------------------------- part 1 */

int32_t op_add(int32_t a, int32_t b)
{
    return a + b;
}

int32_t op_sub(int32_t a, int32_t b)
{
    return a - b;
}

int32_t op_mul(int32_t a, int32_t b)
{
    return a * b;
}

int32_t op_max(int32_t a, int32_t b)
{
    if (a > b)
        return a;
    return b;
}

int32_t apply_op(binary_op_t op, int32_t a, int32_t b, int32_t fallback)
{
    if (op == NULL)
        return fallback;
    return op(a, b);
}

void map_i32(int32_t *values, size_t len, int32_t (*fn)(int32_t))
{
    if (values == NULL || fn == NULL)
        return;
    for (size_t i = 0; i < len; i++) {
        values[i] = fn(values[i]);
    }
}

/* ---------------------------------------------------------------- part 2 */

/* TODO: fill the table. Each entry is {symbol, function}. */
const op_entry_t OP_TABLE[] = {
    {'+', op_add},
    {'-', op_sub},
    {'*', op_mul},
    {'M', op_max},
};
const size_t OP_TABLE_LEN = sizeof OP_TABLE / sizeof OP_TABLE[0];

binary_op_t op_lookup(char symbol)
{
    for (size_t i = 0; i < OP_TABLE_LEN; i++) {
        if (symbol == OP_TABLE[i].symbol)
            return OP_TABLE[i].fn;
    }
    return NULL;
}

bool calc(char symbol, int32_t a, int32_t b, int32_t *result)
{
    if (result == NULL)
        return false;
    binary_op_t p = op_lookup(symbol);
    if (p == NULL)
        return false;
    *result = p(a, b);
    return true;
}

/* ---------------------------------------------------------------- part 3 */

int cmp_ascending(int32_t a, int32_t b)
{
    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    return 0;
}

int cmp_descending(int32_t a, int32_t b)
{
    if (a > b)
        return -1;
    else if (a < b)
        return 1;
    return 0;
}

int cmp_abs(int32_t a, int32_t b)
{
    int64_t abs_a = (a < 0) ? -(int64_t)a : a;
    int64_t abs_b = (b < 0) ? -(int64_t)b : b;
    if (abs_a > abs_b)
        return 1;
    if (abs_a < abs_b)
        return -1;
    return cmp_ascending(a, b);
}

void sort_i32(int32_t *values, size_t len, cmp_fn_t cmp)
{
    if (values == NULL || cmp == NULL || len < 2)
        return;
    for (size_t i = 0; i < len - 1; i++) {
        for (size_t j = 0; j < len - 1 - i; j++) {
            if (cmp(values[j], values[j + 1]) > 0) {
                int32_t tmp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = tmp;
            }
        }
    }
}
