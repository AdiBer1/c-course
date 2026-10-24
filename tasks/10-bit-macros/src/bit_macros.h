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

/* A uint32_t with only bit n set. BIT(0) == 1u, BIT(31) == 0x80000000u. */
#define BIT(n) (0u)

/* reg with bit n set to 1 / cleared to 0 / flipped. */
#define BIT_SET(reg, n)    (reg)
#define BIT_CLEAR(reg, n)  (reg)
#define BIT_TOGGLE(reg, n) (reg)

/* 1u if bit n of reg is 1, else 0u. */
#define BIT_READ(reg, n) (0u)

/*
 * A mask of `width` one-bits starting at bit `pos`.
 * FIELD_MASK(4, 3) == 0x70. Valid: 1 <= width <= 32 and pos + width <= 32,
 * so FIELD_MASK(0, 32) == 0xFFFFFFFFu must work (careful: shifting a 32-bit
 * value by 32 is undefined behaviour).
 */
#define FIELD_MASK(pos, width) (0u)

/* The value of the field: FIELD_GET(0x12345678u, 8, 8) == 0x56. */
#define FIELD_GET(reg, pos, width) (0u)

/*
 * reg with the field replaced by val. Bits of val that do not fit in the
 * field are ignored. FIELD_SET(0xFFFFFFFFu, 4, 4, 0x0) == 0xFFFFFF0Fu.
 */
#define FIELD_SET(reg, pos, width, val) (reg)

#endif /* BIT_MACROS_H */
