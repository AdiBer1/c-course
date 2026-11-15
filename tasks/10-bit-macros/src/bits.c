#include "bits.h"

/* TODO: implement. You may use your macros from bit_macros.h. */

unsigned bits_count_ones(uint32_t x)
{
    unsigned count = 0;

    while (x != 0){
        if ((x & 1u) != 0u){
            count++;
        }

        x >>= 1;
    }

    return count;
}

bool bits_is_power_of_two(uint32_t x)
{
    return (x != 0u) && (bits_count_ones(x) == 1u);
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    return (uint8_t)(((x & 0x0Fu) << 4) | ((x & 0xF0u) >> 4));
}
