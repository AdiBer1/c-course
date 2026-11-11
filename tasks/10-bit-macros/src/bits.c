#include "bits.h"

unsigned bits_count_ones(uint32_t x)
{
    unsigned count = 0;
    while (x > 0u) {
        x &= (x - 1u);
        count++;
    }
    return count;
}

bool bits_is_power_of_two(uint32_t x)
{
    return (x != 0u) && ((x & (x - 1u)) == 0u);
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    return (uint8_t)((uint8_t)(x << 4) | (uint8_t)(x >> 4));
}