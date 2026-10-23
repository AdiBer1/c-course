/*
 * outparams.h - functions that return results through pointers.
 *
 * A C function returns one value. When it must give back several results,
 * the caller passes pointers ("output parameters") where the function
 * stores them, and the return value is kept for a STATUS CODE that says
 * whether it worked. This is how nearly every embedded driver API looks:
 *
 *     status_t adc_read(uint8_t channel, uint16_t *value_out);
 *
 * Rules for every function below:
 *   - check every pointer for NULL before using it
 *   - on any error, do NOT write to any output parameter
 *
 * Do not change this file.
 */
#ifndef OUTPARAMS_H
#define OUTPARAMS_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    OP_OK = 0,
    OP_ERR_NULL = -1,     /* a required pointer was NULL */
    OP_ERR_EMPTY = -2,    /* array length was 0 */
    OP_ERR_DIV_ZERO = -3  /* division by zero */
} op_status_t;

/* Exchange the values *a and *b. If either pointer is NULL, do nothing. */
void swap_i32(int32_t *a, int32_t *b);

/*
 * Find the smallest and largest of values[0..len-1].
 * Returns OP_ERR_NULL if values, min_out or max_out is NULL,
 *         OP_ERR_EMPTY if len == 0 (checked after the NULL checks),
 *         OP_OK otherwise.
 */
op_status_t min_max_i16(const int16_t *values, size_t len, int16_t *min_out, int16_t *max_out);

/*
 * Integer division with remainder: dividend = *quotient * divisor + *remainder.
 * Returns OP_ERR_NULL if quotient or remainder is NULL,
 *         OP_ERR_DIV_ZERO if divisor == 0,
 *         OP_OK otherwise.
 */
op_status_t divmod_u32(uint32_t dividend, uint32_t divisor, uint32_t *quotient, uint32_t *remainder);

/*
 * Sort the three values so that *a <= *b <= *c.
 * Returns OP_ERR_NULL if any pointer is NULL (nothing is changed), else OP_OK.
 */
op_status_t sort3_i32(int32_t *a, int32_t *b, int32_t *c);

#endif /* OUTPARAMS_H */
