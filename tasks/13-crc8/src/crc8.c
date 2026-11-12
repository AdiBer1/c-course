#include "crc8.h"

/* TODO: implement. */

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    crc ^= byte;
    for (int i = 0; i < 8; i++) {
        if ((crc & 0x80u) != 0) {
            crc = (uint8_t)((crc << 1) ^ CRC8_POLY);
        } else {
            crc = (uint8_t)(crc << 1);
        }
    }
    return crc;
}

uint8_t crc8(const uint8_t *data, size_t len)
{
    (void)data;
    (void)len;
    return CRC8_INIT;
}

bool crc8_frame_ok(const uint8_t *frame, size_t len)
{
    (void)frame;
    (void)len;
    return false;
}