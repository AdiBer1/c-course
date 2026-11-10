#include "convert.h"

int32_t c10_to_f10(int32_t c10)
{
    int32_t numerator = c10 * 9;
    int32_t quotient = numerator / 5;
    int32_t remainder = numerator % 5;

    if (remainder >= 3) {
        quotient++;
    } else if (remainder <= -3) {
        quotient--;
    }

    return quotient + 320;
}

int32_t f10_to_c10(int32_t f10)
{
    int32_t numerator = (f10 - 320) * 5;
    int32_t quotient = numerator / 9;
    int32_t remainder = numerator % 9;

    if (remainder >= 5) {
        quotient++;
    } else if (remainder <= -5) {
        quotient--;
    }

    return quotient;
}

uint16_t adc_to_mv(uint16_t raw)
{
    if (raw > 4095) {
        raw = 4095;
    }

    uint32_t numerator = (uint32_t)raw * 3300U;
    uint32_t quotient = numerator / 4095U;
    uint32_t remainder = numerator % 4095U;

    if (remainder * 2U >= 4095U) {
        quotient++;
    }

    return (uint16_t)quotient;
}