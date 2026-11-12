#include "crc8.h"

#include <stddef.h>

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    uint32_t c = (uint32_t)crc ^ (uint32_t)byte;
    for (int i = 0; i < 8; i++) {
        if ((c & 0x80u) != 0u) {
            c = ((c << 1) ^ CRC8_POLY) & 0xFFu;
        } else {
            c = (c << 1) & 0xFFu;
        }
    }
    return (uint8_t)c;
}

uint8_t crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = (uint8_t)CRC8_INIT;
    if (data == NULL) {
        return crc;
    }
    for (size_t i = 0; i < len; i++) {
        crc = crc8_update(crc, data[i]);
    }
    return crc;
}

bool crc8_frame_ok(const uint8_t *frame, size_t len)
{
    if (frame == NULL || len < 2u) {
        return false;
    }
    return crc8(frame, len - 1u) == frame[len - 1u];
}