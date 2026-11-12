#include "bits.h"

unsigned bits_count_ones(uint32_t x)
{
    unsigned count = 0u;

    /* x & (x - 1) clears the lowest 1-bit, so the loop runs once per 1-bit. */
    while (x != 0u) {
        x &= x - 1u;
        count++;
    }
    return count;
}

bool bits_is_power_of_two(uint32_t x)
{
    /* A power of two has exactly one 1-bit, so clearing the lowest 1-bit
     * leaves 0. Zero has no 1-bits at all and is excluded explicitly. */
    return x != 0u && (x & (x - 1u)) == 0u;
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    /* x is promoted to int for the shifts, so the result is an int
     * (e.g. 0xA5 << 4 == 0xA50). The cast back to uint8_t is explicit:
     * it keeps the low 8 bits, which are exactly the swapped nibbles. */
    return (uint8_t)(((unsigned)x << 4) | ((unsigned)x >> 4));
}