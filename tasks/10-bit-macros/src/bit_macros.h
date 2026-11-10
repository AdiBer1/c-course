/*
 * bit_macros.h - macros for single bits and bit fields in a 32-bit value.
 *
 * YOU write these macros (this file is one of your student files).
 * Every macro below is a stub that compiles but gives the wrong result.
 *
 * Rules:
 *   - every macro is an EXPRESSION that yields a uint32_t (BIT_READ: 0u or 1u)
 *   - the macros never modify their arguments; use them like
 *         reg = BIT_SET(reg, 5);
 *   - bit numbers count from 0 (least significant) to 31
 *   - parenthesise every parameter and the whole expression, so that
 *     BIT(1 + 2) and ~BIT(3) work as expected
 *   - an argument may be evaluated more than once: never pass i++ or a
 *     function call with side effects
 *
 * Do not add #include lines for anything except <stdint.h>.
 */

#ifndef BIT_MACROS_H
#define BIT_MACROS_H

#include <stdint.h>

#define BIT(n) \
    ((uint32_t)1u << (n))

#define BIT_SET(reg, n) \
    ((uint32_t)(reg) | BIT(n))

#define BIT_CLEAR(reg, n) \
    ((uint32_t)(reg) & ~BIT(n))

#define BIT_TOGGLE(reg, n) \
    ((uint32_t)(reg) ^ BIT(n))

#define BIT_READ(reg, n) \
    (((uint32_t)(reg) >> (n)) & 1u)

#define FIELD_MASK(pos, width) \
    ((~(uint32_t)0 >> (32u - (width))) << (pos))

#define FIELD_GET(reg, pos, width) \
    (((uint32_t)(reg) & FIELD_MASK(pos, width)) >> (pos))

#define FIELD_SET(reg, pos, width, val) \
    (((uint32_t)(reg) & ~FIELD_MASK(pos, width)) | \
     (((uint32_t)(val) << (pos)) & FIELD_MASK(pos, width)))

#endif /* BIT_MACROS_H */
