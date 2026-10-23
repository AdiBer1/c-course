/*
 * fixed.h - Q16.16 fixed-point arithmetic.
 *
 * Fractional numbers without floating point: an int32_t whose low 16 bits
 * are the fraction. The real value is  raw / 65536.
 *
 *     raw 0x00010000 =  1.0        raw 0x00018000 = 1.5
 *     raw 0xFFFF0000 = -1.0        raw 0x00000001 = 1/65536 (the smallest step)
 *
 * Range: -32768.0 .. +32767.99998. Resolution: 1/65536 = 0.0000153.
 *
 * Used for PID controllers, digital filters and sensor calibration on MCUs
 * without an FPU: addition is a plain integer add, multiplication is an
 * integer multiply plus a shift. Exact, fast and deterministic.
 *
 * Rules for every function:
 *   - ROUND to the nearest representable result, halves away from zero
 *   - SATURATE: a result that does not fit becomes Q16_MAX or Q16_MIN
 *   - never overflow an intermediate value (use int64_t where needed)
 *   - no float, no double
 *
 * Do not change this file.
 */
#ifndef FIXED_H
#define FIXED_H

#include <stdint.h>

typedef int32_t q16_t;

#define Q16_FRAC_BITS 16
#define Q16_ONE       ((q16_t)0x00010000)
#define Q16_MAX       ((q16_t)INT32_MAX)
#define Q16_MIN       ((q16_t)INT32_MIN)

/* Integer -> Q16.16. Outside -32768..32767 saturates. */
q16_t q16_from_int(int32_t x);

/* Q16.16 -> integer, rounded: 1.5 -> 2, -1.5 -> -2, 1.49 -> 1. */
int32_t q16_to_int(q16_t x);

/* Thousandths -> Q16.16, rounded and saturated: 1500 -> 1.5, -250 -> -0.25. */
q16_t q16_from_milli(int32_t milli);

/* Q16.16 -> thousandths, rounded: 1.5 -> 1500. */
int32_t q16_to_milli(q16_t x);

q16_t q16_add(q16_t a, q16_t b);
q16_t q16_sub(q16_t a, q16_t b);

/* a * b, rounded and saturated. */
q16_t q16_mul(q16_t a, q16_t b);

/*
 * a / b, rounded and saturated.
 * Division by zero: a > 0 -> Q16_MAX, a < 0 -> Q16_MIN, a == 0 -> 0.
 */
q16_t q16_div(q16_t a, q16_t b);

#endif /* FIXED_H */
