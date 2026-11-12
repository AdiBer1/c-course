/*
 * bit_macros.h - macros for single bits and bit fields in a 32-bit value.
 *
 * YOU write these macros (this file is one of your student files).
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

/*
 * A uint32_t with only bit n set. BIT(0) == 1u, BIT(31) == 0x80000000u.
 * The 1 is an unsigned 32-bit value: `1 << 31` on a signed int would shift
 * a 1 into the sign bit, which is undefined behaviour.
 */
#define BIT(n) ((uint32_t)1u << (n))

/* reg with bit n set to 1 / cleared to 0 / flipped. */
#define BIT_SET(reg, n)    ((uint32_t)((uint32_t)(reg) | BIT(n)))
#define BIT_CLEAR(reg, n)  ((uint32_t)((uint32_t)(reg) & (uint32_t)~BIT(n)))
#define BIT_TOGGLE(reg, n) ((uint32_t)((uint32_t)(reg) ^ BIT(n)))

/* 1u if bit n of reg is 1, else 0u. */
#define BIT_READ(reg, n) ((uint32_t)(((uint32_t)(reg) >> (n)) & 1u))

/*
 * A mask of `width` one-bits starting at bit `pos`.
 * FIELD_MASK(4, 3) == 0x70. Valid: 1 <= width <= 32 and pos + width <= 32.
 *
 * Built by shifting all-ones RIGHT by (32 - width): for width == 32 that is
 * a shift by 0, so FIELD_MASK(0, 32) == 0xFFFFFFFFu works. The naive
 * ((1u << width) - 1u) would shift by 32, which is undefined behaviour.
 * pos + width <= 32 and width >= 1 means pos <= 31, so the left shift is safe.
 */
#define FIELD_MASK(pos, width) \
    ((uint32_t)((0xFFFFFFFFu >> (32u - (uint32_t)(width))) << (pos)))

/* The value of the field: FIELD_GET(0x12345678u, 8, 8) == 0x56. */
#define FIELD_GET(reg, pos, width) \
    ((uint32_t)(((uint32_t)(reg) >> (pos)) & FIELD_MASK(0u, (width))))

/*
 * reg with the field replaced by val. Bits of val that do not fit in the
 * field are ignored. FIELD_SET(0xFFFFFFFFu, 4, 4, 0x0) == 0xFFFFFF0Fu.
 *
 *   1. (reg & ~mask)            clear the old field
 *   2. ((val << pos) & mask)    move val into place, drop bits that do not fit
 *   3. OR the two together
 */
#define FIELD_SET(reg, pos, width, val)                              \
    ((uint32_t)(((uint32_t)(reg) & (uint32_t)~FIELD_MASK((pos), (width))) | \
                (((uint32_t)(val) << (pos)) & FIELD_MASK((pos), (width)))))

#endif /* BIT_MACROS_H */