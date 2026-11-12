#include "bits.h"

/* TODO: implement. You may use your macros from bit_macros.h. */

unsigned bits_count_ones(uint32_t x)
{
    unsigned count = 0;
    while (x != 0u) {
        if (x & 1u) {
            count++;
        }
        x = x >> 1;
    }
    return count;
}

bool bits_is_power_of_two(uint32_t x)
{
    return bits_count_ones(x) == 1u;
}
uint8_t bits_swap_nibbles(uint8_t x)
{
    uint32_t v = x;
    return (uint8_t)(((v << 4) | (v >> 4)) & 0xFFu);
}