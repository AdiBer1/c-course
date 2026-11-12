#include "crc8.h"

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    crc ^= byte;
    for (uint8_t bit = 0u; bit < 8u; bit++) {
        if ((crc & 0x80u) != 0u) {
            crc = (uint8_t)((uint8_t)(crc << 1u) ^ CRC8_POLY);
        } else {
            crc = (uint8_t)(crc << 1u);
        }
    }
    return crc;
}

uint8_t crc8(const uint8_t *data, size_t len)
{
    if (data == NULL || len == 0u) {
        return CRC8_INIT;
    }
    uint8_t crc = CRC8_INIT;
    for (size_t i = 0u; i < len; i++) {
        crc = crc8_update(crc, data[i]);
    }
    return crc;
}

bool crc8_frame_ok(const uint8_t *frame, size_t len)
{
    if (frame == NULL || len < 2u) {
        return false;
    }
    uint8_t computed = crc8(frame, len - 1u);
    return computed == frame[len - 1u];
}