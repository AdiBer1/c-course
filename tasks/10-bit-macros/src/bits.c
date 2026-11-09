#include "bits.h"

unsigned bits_count_ones(uint32_t x)
{
    unsigned n = 0;
    while (x) { n += x & 1u; x >>= 1; }
    return n;
}

bool bits_is_power_of_two(uint32_t x)
{
    return x != 0u && (x & (x - 1u)) == 0u;
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    return (uint8_t)(((x & 0x0Fu) << 4) | ((x & 0xF0u) >> 4));
}
