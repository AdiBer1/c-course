#include "saturate.h"

/* TODO: implement every function. Each one currently returns 0. */

uint8_t sat_add_u8(uint8_t a, uint8_t b)
{
  
    uint16_t sum = a + b;
    if (sum > UINT8_MAX) {
        return UINT8_MAX;
    }
    return (uint8_t)sum;
}

uint8_t sat_sub_u8(uint8_t a, uint8_t b)
{
  
    if(a<b){
        return 0;
    }
    return (uint8_t)a-b;
}

int16_t sat_add_i16(int16_t a, int16_t b)
{
    int32_t sum = (int32_t)a + (int32_t)b;
    if (sum > INT16_MAX) {
        return INT16_MAX;
    } else if (sum < INT16_MIN) {
        return INT16_MIN;
    }
    return (int16_t)sum;
}

int16_t sat_sub_i16(int16_t a, int16_t b)
{
    int32_t diff = (int32_t)a - (int32_t)b;
    if (diff > INT16_MAX) {
        return INT16_MAX;}
    if (diff < INT16_MIN) {
        return INT16_MIN;
    }   

    return (int16_t)diff;
}
