/*
 * ops.h - a first look at function pointers.
 *
 * A function has an address, just like a variable. A FUNCTION POINTER holds
 * that address, so code can decide at run time which function to call:
 *
 *     typedef int32_t (*binary_op_t)(int32_t a, int32_t b);
 *
 *     binary_op_t op = op_add;    // no parentheses: the function's address
 *     int32_t r = op(2, 3);       // call through the pointer -> 5
 *
 * Three classic uses, one per part of this task:
 *   1. pass behaviour as an argument           (apply_op, map_i32)
 *   2. a table that maps a key to a function   (op_lookup, calc)
 *   3. a comparator that customises sorting    (sort_i32), like qsort()
 *
 * In firmware the same idea gives you interrupt vector tables, command
 * dispatch tables, state machines with one function per state, and the
 * driver interfaces of the next task.
 *
 * Do not change this file.
 */
#ifndef OPS_H
#define OPS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---------------------------------------------------------------- part 1 */

typedef int32_t (*binary_op_t)(int32_t a, int32_t b);

/* Plain functions that match binary_op_t. Callers keep values small: no overflow checks. */
int32_t op_add(int32_t a, int32_t b);
int32_t op_sub(int32_t a, int32_t b);
int32_t op_mul(int32_t a, int32_t b);
int32_t op_max(int32_t a, int32_t b);

/* Call op(a, b). If op is NULL, return `fallback` instead. */
int32_t apply_op(binary_op_t op, int32_t a, int32_t b, int32_t fallback);

/* Replace every values[i] with fn(values[i]). Nothing happens if values or fn is NULL. */
void map_i32(int32_t *values, size_t len, int32_t (*fn)(int32_t));

/* ---------------------------------------------------------------- part 2 */

typedef struct {
    char symbol;     /* '+', '-', '*', 'M' (max) */
    binary_op_t fn;
} op_entry_t;

/*
 * The table of operations. YOU fill it in ops.c so that it maps
 * '+' -> op_add, '-' -> op_sub, '*' -> op_mul, 'M' -> op_max.
 */
extern const op_entry_t OP_TABLE[];
extern const size_t OP_TABLE_LEN;

/* Find the function for `symbol` in OP_TABLE (a loop, not a switch). Unknown -> NULL. */
binary_op_t op_lookup(char symbol);

/*
 * Compute `a symbol b` using op_lookup. Returns true and stores the result
 * in *result, or returns false (result unchanged) for an unknown symbol or
 * a NULL result pointer.
 */
bool calc(char symbol, int32_t a, int32_t b, int32_t *result);

/* ---------------------------------------------------------------- part 3 */

/* Comparator: < 0 if a should come before b, > 0 if after, 0 if equal. */
typedef int (*cmp_fn_t)(int32_t a, int32_t b);

int cmp_ascending(int32_t a, int32_t b);
int cmp_descending(int32_t a, int32_t b);
/* Order by absolute value, smaller first; equal absolute values: negative first. */
int cmp_abs(int32_t a, int32_t b);

/*
 * Sort values[0..len-1] so that cmp(values[i], values[i+1]) <= 0 for every i.
 * Use any simple algorithm (insertion sort is fine). The comparator is the
 * ONLY place where two values are compared. Nothing happens if values or
 * cmp is NULL.
 */
void sort_i32(int32_t *values, size_t len, cmp_fn_t cmp);

#endif /* OPS_H */
