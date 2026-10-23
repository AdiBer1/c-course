#include "ops.h"

/* ---------------------------------------------------------------- part 1 */

int32_t op_add(int32_t a, int32_t b)
{
    (void)a;
    (void)b;
    return 0; /* TODO */
}

int32_t op_sub(int32_t a, int32_t b)
{
    (void)a;
    (void)b;
    return 0; /* TODO */
}

int32_t op_mul(int32_t a, int32_t b)
{
    (void)a;
    (void)b;
    return 0; /* TODO */
}

int32_t op_max(int32_t a, int32_t b)
{
    (void)a;
    (void)b;
    return 0; /* TODO */
}

int32_t apply_op(binary_op_t op, int32_t a, int32_t b, int32_t fallback)
{
    (void)op;
    (void)a;
    (void)b;
    (void)fallback;
    return 0; /* TODO */
}

void map_i32(int32_t *values, size_t len, int32_t (*fn)(int32_t))
{
    (void)values;
    (void)len;
    (void)fn;
    /* TODO */
}

/* ---------------------------------------------------------------- part 2 */

/* TODO: fill the table. Each entry is {symbol, function}. */
const op_entry_t OP_TABLE[] = {
    {'+', NULL},
    {'-', NULL},
    {'*', NULL},
    {'M', NULL},
};
const size_t OP_TABLE_LEN = sizeof OP_TABLE / sizeof OP_TABLE[0];

binary_op_t op_lookup(char symbol)
{
    (void)symbol;
    return NULL; /* TODO */
}

bool calc(char symbol, int32_t a, int32_t b, int32_t *result)
{
    (void)symbol;
    (void)a;
    (void)b;
    (void)result;
    return false; /* TODO */
}

/* ---------------------------------------------------------------- part 3 */

int cmp_ascending(int32_t a, int32_t b)
{
    (void)a;
    (void)b;
    return 0; /* TODO */
}

int cmp_descending(int32_t a, int32_t b)
{
    (void)a;
    (void)b;
    return 0; /* TODO */
}

int cmp_abs(int32_t a, int32_t b)
{
    (void)a;
    (void)b;
    return 0; /* TODO */
}

void sort_i32(int32_t *values, size_t len, cmp_fn_t cmp)
{
    (void)values;
    (void)len;
    (void)cmp;
    /* TODO */
}
