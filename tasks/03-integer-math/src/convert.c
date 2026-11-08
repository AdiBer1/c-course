#include "convert.h"

static int32_t div_round(int32_t num, int32_t den) {
    if (num >= 0) {
        return (num + den / 2) / den;
    } else {
        return (num - den / 2) / den;
    }
}

static uint32_t div_round_u32(uint32_t num, uint32_t den) {
    return (num + den / 2) / den;
}

int32_t c10_to_f10(int32_t c10) {
    int32_t scaled = c10 * 9;
    return div_round(scaled, 5) + 320;
}

int32_t f10_to_c10(int32_t f10) {
    int32_t diff = f10 - 320;
    int32_t scaled = diff * 5;
    return div_round(scaled, 9);
}

uint16_t adc_to_mv(uint16_t raw) {
    if (raw > 4095) {
        raw = 4095;
    }
    uint32_t prod = (uint32_t)raw * 3300;
    return (uint16_t)div_round_u32(prod, 4095);
}
