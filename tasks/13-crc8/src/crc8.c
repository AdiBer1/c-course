#include "crc8.h"

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    int bit;

    crc = (uint8_t)(crc ^ byte);
    for (bit = 0; bit < 8; bit++) {
        if ((crc & 0x80u) != 0u) {
            crc = (uint8_t)((crc << 1) ^ CRC8_POLY);
        } else {
            crc = (uint8_t)(crc << 1);
        }
    }
    return crc;
}

uint8_t crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = CRC8_INIT;
    size_t i;

    if (data == NULL) {
        return CRC8_INIT;
    }
    for (i = 0; i < len; i++) {
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