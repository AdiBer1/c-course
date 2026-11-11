#include "bits.h"

/* TODO: implement. You may use your macros from bit_macros.h. */

unsigned bits_count_ones(uint32_t x)
{
    unsigned count =
        0; //the trik of karnigan, this counts the number of 1s in the binary representation of x, by repeatedly clearing the least significant 1 bit and counting how many times we do that until x becomes 0, claude tought me this
    while (x != 0u) {
        x &= (x - 1u);
        count++;
    }
    return count;
}

bool bits_is_power_of_two(uint32_t x)
{

    return (x != 0u)
           && ((x & (x - 1u))
               == 0u); //this checks if x is a power of two by checking if it has only one bit set, which is true for powers of two, and also checks if x is not zero, because zero is not a power of two, the same trik of karnigan, this is a common trick to check if a number is a power of two
}

uint8_t bits_swap_nibbles(uint8_t x)
{
    return (
        uint8_t)(((x & 0x0Fu) << 4)
                 | (x
                    >> 4)); //this swaps the high and low nibbles of x by masking the low nibble, shifting it to the left by 4 bits, and then oring it with the high nibble shifted to the right by 4 bits, this effectively swaps the two nibbles
}
