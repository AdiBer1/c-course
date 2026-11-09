#include "crc8.h"

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    crc ^= byte;
    for (int i = 0; i < 8; i++) {
        if (crc & 0x80u)
            crc = (uint8_t)((crc << 1) ^ CRC8_POLY);
        else
            crc = (uint8_t)(crc << 1);
    }
    return crc;
}

uint8_t crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = CRC8_INIT;
    if (!data) return crc;
    for (size_t i = 0; i < len; i++)
        crc = crc8_update(crc, data[i]);
    return crc;
}

bool crc8_frame_ok(const uint8_t *frame, size_t len)
{
    if (!frame || len < 2) return false;
    uint8_t expected = crc8(frame, len - 1);
    return expected == frame[len - 1];
}
