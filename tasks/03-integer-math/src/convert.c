#include "convert.h"

/* TODO: implement every function. Each one currently returns 0. */

int32_t c10_to_f10(int32_t c10)
{
    int32_t f10 = (c10 * 9 + (c10 >= 0 ? 2 : -2)) / 5 + 320;
    return f10;
}

int32_t f10_to_c10(int32_t f10)
{

    int32_t c10 = ((f10 - 320) * 5+((f10-320)>=0?4:-4)) / 9; // Subtract 320, multiply by 5, then divide by 9
    return c10;
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095) {
        raw = 4095; // Clamp to maximum value
    }
    uint16_t mv = (raw * 3300 + 2047) / 4095; // Multiply by 3300 and divide by 4095, rounding to nearest
    return mv;
}
