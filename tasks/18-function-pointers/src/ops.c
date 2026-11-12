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
    return (a > b) ? a : b;
}

int32_t apply_op(binary_op_t op, int32_t a, int32_t b, int32_t fallback)
{
    /* Never call a NULL function pointer: it would jump to address 0. */
    if (op == NULL) {
        return fallback;
    }
    return op(a, b); /* calls whichever function `op` points to */
}

void map_i32(int32_t *values, size_t len, int32_t (*fn)(int32_t))
{
    if (values == NULL || fn == NULL) {
        return;
    }
    for (size_t i = 0u; i < len; i++) {
        values[i] = fn(values[i]);
    }
}

/* ---------------------------------------------------------------- part 2 */

/* Each entry stores the function's ADDRESS (op_add, not op_add()). */
const op_entry_t OP_TABLE[] = {
    {'+', op_add},
    {'-', op_sub},
    {'*', op_mul},
    {'M', op_max},
};
const size_t OP_TABLE_LEN = sizeof OP_TABLE / sizeof OP_TABLE[0];

binary_op_t op_lookup(char symbol)
{
    /* Adding a command means adding one table line; this loop never changes. */
    for (size_t i = 0u; i < OP_TABLE_LEN; i++) {
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
    binary_op_t fn = op_lookup(symbol);
    if (fn == NULL) {
        return false; /* unknown symbol: *result unchanged */
    }
    *result = fn(a, b);
    return true;
}

/* ---------------------------------------------------------------- part 3 */

/*
 * (a > b) - (a < b) gives -1, 0 or 1 and never overflows, unlike a - b
 * (e.g. INT32_MIN - 1 overflows and would give the wrong sign).
 */
int cmp_ascending(int32_t a, int32_t b)
{
    return (a > b) - (a < b);
}

int cmp_descending(int32_t a, int32_t b)
{
    return (a < b) - (a > b);
}

int cmp_abs(int32_t a, int32_t b)
{
    /* |INT32_MIN| does not fit in int32_t, so compute the magnitudes in 64 bits. */
    int64_t abs_a = (a < 0) ? -(int64_t)a : (int64_t)a;
    int64_t abs_b = (b < 0) ? -(int64_t)b : (int64_t)b;

    if (abs_a != abs_b) {
        return (abs_a > abs_b) - (abs_a < abs_b);
    }
    /* Same magnitude (e.g. -3 and 3): the negative one comes first. */
    return (a > b) - (a < b);
}

void sort_i32(int32_t *values, size_t len, cmp_fn_t cmp)
{
    if (values == NULL || cmp == NULL) {
        return;
    }
    /*
     * Insertion sort: take each value and move it left past every value that
     * should come after it. The comparator is the only place values are
     * compared, so the same code sorts in any order.
     */
    for (size_t i = 1u; i < len; i++) {
        int32_t current = values[i];
        size_t j = i;
        while (j > 0u && cmp(values[j - 1u], current) > 0) {
            values[j] = values[j - 1u];
            j--;
        }
        values[j] = current;
    }
}