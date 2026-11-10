#ifndef BIT_MACROS_H
#define BIT_MACROS_H

#include <stdint.h>

/* A uint32_t with only bit n set. BIT(0) == 1u, BIT(31) == 0x80000000u. */
#define BIT(n) ((uint32_t)1u << (n))

/* reg with bit n set to 1 / cleared to 0 / flipped. */
#define BIT_SET(reg, n)    ((uint32_t)(reg) | BIT(n))
#define BIT_CLEAR(reg, n)  ((uint32_t)(reg) & ~BIT(n))
#define BIT_TOGGLE(reg, n) ((uint32_t)(reg) ^ BIT(n))

/* 1u if bit n of reg is 1, else 0u. */
#define BIT_READ(reg, n) (((uint32_t)(reg) >> (n)) & 1u)

/*
 * A mask of `width` one-bits starting at bit `pos`.
 * Safe shift logic: avoid 1u << 32 when width == 32.
 */
#define FIELD_MASK(pos, width) \
    (((width) == 32u) ? 0xFFFFFFFFu : (((uint32_t)1u << (width)) - 1u) << (pos))

/* The value of the field: FIELD_GET(0x12345678u, 8, 8) == 0x56. */
#define FIELD_GET(reg, pos, width) \
    (((uint32_t)(reg) >> (pos)) & (((width) == 32u) ? 0xFFFFFFFFu : (((uint32_t)1u << (width)) - 1u)))

/*
 * reg with the field replaced by val. Bits of val that do not fit in the
 * field are ignored.
 */
#define FIELD_SET(reg, pos, width, val) \
    (((uint32_t)(reg) & ~FIELD_MASK((pos), (width))) | \
     ((((uint32_t)(val)) & (((width) == 32u) ? 0xFFFFFFFFu : (((uint32_t)1u << (width)) - 1u))) << (pos)))

#endif /* BIT_MACROS_H */