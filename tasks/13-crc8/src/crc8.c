#include "crc8.h"

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    crc = (uint8_t)(crc ^ byte);

    for (unsigned int i = 0u; i < 8u; i++) {
        if ((crc & 0x80u) != 0u) {
            crc = (uint8_t)(((unsigned int)crc << 1) ^ CRC8_POLY);
        } else {
            crc = (uint8_t)((unsigned int)crc << 1);
        }
    }

    return crc;
}

uint8_t crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = CRC8_INIT;

    if (data == NULL) {
        return CRC8_INIT;
    }

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

    return crc8(frame, len - 1u) == frame[len - 1u];
}