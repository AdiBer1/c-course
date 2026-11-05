#include "bits.h"
#include "bit_macros.h"

/* TODO: implement. You may use your macros from bit_macros.h. */

unsigned bits_count_ones(uint32_t x)
{
    unsigned count = 0;
    for (unsigned i = 0; i < 32; i++) {
        count += BIT_READ(x, i);
    }
    return count;
}

bool bits_is_power_of_two(uint32_t x)
{
    return (x != 0) && ((x & (x - 1u)) == 0);
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    uint32_t low_nibble  = FIELD_GET(x, 0, 4); // חילוץ 4 הביטים הנמוכים
    uint32_t high_nibble = FIELD_GET(x, 4, 4); // חילוץ 4 הביטים הגבוהים
    
    return (uint8_t)((low_nibble << 4) | high_nibble);
}
