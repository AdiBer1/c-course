#include "bits.h"

/* TODO: implement. You may use your macros from bit_macros.h. */

unsigned bits_count_ones(uint32_t x)
{
    unsigned counter = 0u;
    for (int i = 0; i < 32; i++) {
        if (BIT_READ(x, i))
            counter++;
    }
    return counter;
}

bool bits_is_power_of_two(uint32_t x)
{
    if (x == 0)
        return false;
    return (x & (x - 1)) == 0;
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    uint8_t low = (uint8_t)((x & 0x0F) << 4);
    uint8_t high = (uint8_t)((x & 0xF0) >> 4);

    return low | high;
}
