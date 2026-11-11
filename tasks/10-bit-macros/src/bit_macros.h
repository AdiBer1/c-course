#ifndef BIT_MACROS_H
#define BIT_MACROS_H

#include <stdint.h>

/* A uint32_t with only bit n set. BIT(0) == 1u, BIT(31) == 0x80000000u. */
#define BIT(n) (1u << (n))
/* reg with bit n set to 1 / cleared to 0 / flipped. */
#define BIT_SET(reg, n)    ((reg) | BIT(n))
#define BIT_CLEAR(reg, n)  ((reg) & ~BIT(n))
#define BIT_TOGGLE(reg, n) ((reg) ^ BIT(n))

/* 1u if bit n of reg is 1, else 0u. */
#define BIT_READ(reg, n) (((reg) >> (n)) & 1u)

/*
 * A mask of `width` one-bits starting at bit `pos`.
 * FIELD_MASK(4, 3) == 0x70. Valid: 1 <= width <= 32 and pos + width <= 32,
 * so FIELD_MASK(0, 32) == 0xFFFFFFFFu must work (careful: shifting a 32-bit
 * value by 32 is undefined behaviour).
 */
#define FIELD_MASK(pos, width) \
    (((width) == 32u) ? 0xFFFFFFFFu : (((1u << (width)) - 1u) << (pos)))

/* The value of the field: FIELD_GET(0x12345678u, 8, 8) == 0x56. */
#define FIELD_GET(reg, pos, width) (((reg) >> (pos)) & FIELD_MASK(0, width))

/*
 * reg with the field replaced by val. Bits of val that do not fit in the
 * field are ignored. FIELD_SET(0xFFFFFFFFu, 4, 4, 0x0) == 0xFFFFFF0Fu.
 */
#define FIELD_SET(reg, pos, width, val) \
    (((reg) & ~FIELD_MASK((pos), (width))) | (((val) & FIELD_MASK(0, (width))) << (pos)))

#endif /* BIT_MACROS_H */