/*
 * saturate.h - saturating arithmetic for fixed-width integers.
 *
 * Normal integer arithmetic wraps around (unsigned) or is undefined behaviour
 * (signed) when the result does not fit. Saturating arithmetic instead clamps
 * the result to the largest or smallest value the type can hold:
 *
 *     sat_add_u8(200, 100) == 255     (not 44)
 *     sat_sub_u8(10, 20)   == 0       (not 246)
 *     sat_add_i16(30000, 10000) == 32767
 *
 * DSP chips and sensor code use this so that a value that is "too loud"
 * stays at the maximum instead of suddenly becoming very small.
 *
 * Do not change this file.
 */
#ifndef SATURATE_H
#define SATURATE_H

#include <stdint.h>

/* a + b, clamped to [0, UINT8_MAX]. */
uint8_t sat_add_u8(uint8_t a, uint8_t b);

/* a - b, clamped to [0, UINT8_MAX]. */
uint8_t sat_sub_u8(uint8_t a, uint8_t b);

/* a + b, clamped to [INT16_MIN, INT16_MAX]. */
int16_t sat_add_i16(int16_t a, int16_t b);

/* a - b, clamped to [INT16_MIN, INT16_MAX]. */
int16_t sat_sub_i16(int16_t a, int16_t b);

#endif /* SATURATE_H */
