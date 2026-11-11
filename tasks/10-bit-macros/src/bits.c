#include "bits.h"

unsigned bits_count_ones(uint32_t x)
{
    unsigned count = 0u;

    while (x != 0u) {
        x &= x - 1u; /* clears the lowest 1-bit */
        count++;
    }
    return count;
}

bool bits_is_power_of_two(uint32_t x)
{
    /* a power of two has exactly one 1-bit, so x & (x - 1) removes it */
    return (x != 0u) && ((x & (x - 1u)) == 0u);
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    /* x is promoted to int; the cast keeps only the low 8 bits */
    return (uint8_t)(((unsigned)x << 4) | ((unsigned)x >> 4));
}