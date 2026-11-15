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
    /* A power of two has exactly one 1-bit, so x & (x - 1) clears it to 0. */
    return x != 0u && (x & (x - 1u)) == 0u;
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    uint32_t v = x;
    return (uint8_t)(((v << 4) | (v >> 4)) & 0xFFu);
}
