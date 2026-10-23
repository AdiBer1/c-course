/*
 * bits.h - bit manipulation helpers.
 *
 * Two parts:
 *   bit_macros.h  the macros (YOU write them in this task)
 *   bits.c        three small functions (you write them too)
 *
 * Do not change this file.
 */
#ifndef BITS_H
#define BITS_H

#include <stdbool.h>
#include <stdint.h>

#include "bit_macros.h"

/* Number of bits that are 1 in x. bits_count_ones(0xF0F0) == 8. */
unsigned bits_count_ones(uint32_t x);

/* true if exactly one bit of x is set (1, 2, 4, ... 0x80000000). 0 is not a power of two. */
bool bits_is_power_of_two(uint32_t x);

/* Swap the high and low 4 bits: bits_swap_nibbles(0xA5) == 0x5A. */
uint8_t bits_swap_nibbles(uint8_t x);

#endif /* BITS_H */
