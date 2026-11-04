#include "saturate.h"

/* TODO: implement every function. Each one currently returns 0. */

uint8_t sat_add_u8(uint8_t a, uint8_t b)
{
    uint8_t result;
    if ((a+b)>UINT8_MAX) result =UINT8_MAX;
    else result=a+b;
    return result;
}

uint8_t sat_sub_u8(uint8_t a, uint8_t b)
{
    uint8_t result;
    if ((a-b)<0) result =0;
    else result=a-b;
    return result;
}

int16_t sat_add_i16(int16_t a, int16_t b)
{
     int16_t result;
    if ((a+b)>INT16_MAX) result =INT16_MAX;
    else if((a+b)<INT16_MIN) result=INT16_MIN;
    else result=a+b;
    return result;
}

int16_t sat_sub_i16(int16_t a, int16_t b)
{
    int16_t result;
    if ((a-b)>INT16_MAX) result =INT16_MAX;
    else if ((a-b)<INT16_MIN) result =INT16_MIN;
    else result=a-b;
    return result;
}
