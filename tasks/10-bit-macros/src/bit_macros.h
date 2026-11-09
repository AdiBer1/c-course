#ifndef BIT_MACROS_H
#define BIT_MACROS_H

#include <stdint.h>

#define BIT(n) ((uint32_t)1u << (n))

#define BIT_SET(reg, n)    ((uint32_t)(reg) | BIT(n))
#define BIT_CLEAR(reg, n)  ((uint32_t)(reg) & ~BIT(n))
#define BIT_TOGGLE(reg, n) ((uint32_t)(reg) ^ BIT(n))

#define BIT_READ(reg, n) (((uint32_t)(reg) >> (n)) & 1u)

#define FIELD_MASK(pos, width) \
    ((((width) == 32u) ? 0xFFFFFFFFu : (((uint32_t)1u << (width)) - 1u)) << (pos))

#define FIELD_GET(reg, pos, width) \
    (((uint32_t)(reg) >> (pos)) & (((width) == 32u) ? 0xFFFFFFFFu : (((uint32_t)1u << (width)) - 1u)))

#define FIELD_SET(reg, pos, width, val) \
    (((uint32_t)(reg) & ~FIELD_MASK((pos), (width))) | \
     (((uint32_t)(val) & (((width) == 32u) ? 0xFFFFFFFFu : (((uint32_t)1u << (width)) - 1u))) << (pos)))

#endif /* BIT_MACROS_H */
