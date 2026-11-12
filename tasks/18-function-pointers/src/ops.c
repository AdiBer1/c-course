#include "ops.h"
#include <stdlib.h>

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
    return (a > b) ? a : b;
}

int32_t apply_op(binary_op_t op, int32_t a, int32_t b, int32_t fallback)
{
    if (op == NULL) {
        return fallback;
    }
    return op(a, b);
}

void map_i32(int32_t *values, size_t len, int32_t (*fn)(int32_t))
{
    if (values == NULL || fn == NULL) {
        return;
    }
    for (size_t i = 0; i < len; i++) {
        values[i] = fn(values[i]);
    }
}

/* ---------------------------------------------------------------- part 2 */

const op_entry_t OP_TABLE[] = {
    {'+', op_add},
    {'-', op_sub},
    {'*', op_mul},
    {'M', op_max}
};

const size_t OP_TABLE_LEN = sizeof(OP_TABLE) / sizeof(OP_TABLE[0]);

binary_op_t op_lookup(char symbol)
{
    for (size_t i = 0; i < OP_TABLE_LEN; i++) {
        if (OP_TABLE[i].symbol == symbol) {
            return OP_TABLE[i].fn;
        }
    }
    return NULL;
}

bool calc(char symbol, int32_t a, int32_t b, int32_t *result)
{
    if (result == NULL) {
        return false;
    }
    binary_op_t op = op_lookup(symbol);
    if (op == NULL) {
        return false;
    }
    *result = op(a, b);
    return true;
}

/* ---------------------------------------------------------------- part 3 */

int cmp_ascending(int32_t a, int32_t b)
{
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

int cmp_descending(int32_t a, int32_t b)
{
    if (a > b) {
        return -1;
    }
    if (a < b) {
        return 1;
    }
    return 0;
}

int cmp_abs(int32_t a, int32_t b)
{
    int64_t abs_a = (a < 0) ? -(int64_t)a : (int64_t)a;
    int64_t abs_b = (b < 0) ? -(int64_t)b : (int64_t)b;

    if (abs_a < abs_b) {
        return -1;
    }
    if (abs_a > abs_b) {
        return 1;
    }

    /* Equal absolute values: negative first */
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}

void sort_i32(int32_t *values, size_t len, cmp_fn_t cmp)
{
    if (values == NULL || cmp == NULL || len < 2) {
        return;
    }

    /* Insertion sort implementation using comparator */
    for (size_t i = 1; i < len; i++) {
        int32_t key = values[i];
        size_t j = i;
        while (j > 0 && cmp(values[j - 1], key) > 0) {
            values[j] = values[j - 1];
            j--;
        }
        values[j] = key;
    }
}