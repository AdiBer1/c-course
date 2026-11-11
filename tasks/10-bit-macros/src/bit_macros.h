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
#define BIT(n) ((uint32_t)1u << (n))

/* reg with bit n set to 1 / cleared to 0 / flipped. */
#define BIT_SET(reg, n)                                                                            \
    ((uint32_t)(reg) | BIT(n)) //Or (reg) | BIT(n) - for each bit, so it will make all 1
#define BIT_CLEAR(reg, n)                                                                          \
    ((uint32_t)(reg) & ~BIT(n)) //And with 0 for each bit int he reg, so all will be 0
#define BIT_TOGGLE(reg, n)                                                                         \
    ((uint32_t)(reg) ^ BIT(n)) //Xor with 1 for each bit in the reg, so it will flip all bits

/* 1u if bit n of reg is 1, else 0u. */
#define BIT_READ(reg, n)                                                                           \
    (((uint32_t)(reg) >> (n))                                                                      \
     & 1u) //moving the bit to the LSB, and then anding with 1u to get the value of that bit - all the other bits of the reg will be 0 because 1u is 0x00000001, so it will only keep the LSB and all other bits will be 0

/*
 * A mask of `width` one-bits starting at bit `pos`.
 * FIELD_MASK(4, 3) == 0x70. Valid: 1 <= width <= 32 and pos + width <= 32,
 * so FIELD_MASK(0, 32) == 0xFFFFFFFFu must work (careful: shifting a 32-bit
 * value by 32 is undefined behaviour).
 */
#define FIELD_MASK(pos, width) ((0xFFFFFFFFu >> (32u - (uint32_t)(width))) << (pos))

/* The value of the field: FIELD_GET(0x12345678u, 8, 8) == 0x56. */
#define FIELD_GET(reg, pos, width)                                                                 \
    (((uint32_t)(reg) & FIELD_MASK(pos, width))                                                    \
     >> (pos)) //Anding with the mask to get the value of the field, and then shifting it to the right to get the value of the field in the LSB position

/*
 * reg with the field replaced by val. Bits of val that do not fit in the
 * field are ignored. FIELD_SET(0xFFFFFFFFu, 4, 4, 0x0) == 0xFFFFFF0Fu.
 */
#define FIELD_SET(reg, pos, width, val)                                                            \
    (((uint32_t)(reg) & ~FIELD_MASK(pos, width))                                                   \
     | (((uint32_t)(val) << (pos))                                                                 \
        & FIELD_MASK(                                                                              \
            pos,                                                                                   \
            width))) //Anding with the negation of the mask to clear the field, and then oring with the value shifted to the correct position and masked to fit in the field

#endif /* BIT_MACROS_H */
