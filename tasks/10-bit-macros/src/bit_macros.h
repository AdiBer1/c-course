#ifndef BIT_MACROS_H
#define BIT_MACROS_H

#include <stdint.h>

#define BIT(n) (1u << (n))
/* reg with bit n set to 1 / cleared to 0 / flipped. */
#define BIT_SET(reg, n)    ((reg) | BIT(n))
#define BIT_CLEAR(reg, n)  ((reg) & ~BIT(n))
#define BIT_TOGGLE(reg, n) ((reg) ^ BIT(n))

/* 1u if bit n of reg is 1, else 0u. */
#define BIT_READ(reg, n) (((reg) >> (n)) & 1u)

#define FIELD_MASK(pos, width) \
    (((width) == 32u) ? 0xFFFFFFFFu : (((1u << (width)) - 1u) << (pos)))

/* The value of the field: FIELD_GET(0x12345678u, 8, 8) == 0x56. */
#define FIELD_GET(reg, pos, width) (((reg) >> (pos)) & FIELD_MASK(0, width))

#define FIELD_SET(reg, pos, width, val) \
    (((reg) & ~FIELD_MASK((pos), (width))) | (((val) & FIELD_MASK(0, (width))) << (pos)))

#endif /* BIT_MACROS_H */