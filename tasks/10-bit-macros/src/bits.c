#include "bits.h"
#include "bit_macros.h"

unsigned bits_count_ones(uint32_t x)
{
    unsigned count = 0;
    while (x > 0) {
        if (x & 1u) {
            count++;
        }
        x >>= 1u;
    }
    return count;
}

bool bits_is_power_of_two(uint32_t x)
{
    /* A power of two is strictly greater than 0 and has exactly one bit set.
     * x & (x - 1) clears the lowest set bit. If it becomes 0, it's a power of two. */
    return (x > 0) && ((x & (x - 1u)) == 0);
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    /* Swap the lower 4 bits (nibble) with the upper 4 bits */
    return (uint8_t)(((x & 0x0Fu) << 4) | ((x & 0xF0u) >> 4));
}