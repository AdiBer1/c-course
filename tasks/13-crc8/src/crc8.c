#include "crc8.h"

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    crc ^= byte;
    for (unsigned i = 0; i < 8u; i++) {
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
    if (data == NULL) {
        return CRC8_INIT;
    }

    uint8_t crc = CRC8_INIT;
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

    size_t data_len = len - 1u;
    uint8_t expected_crc = frame[data_len];
    uint8_t computed_crc = crc8(frame, data_len);

    return computed_crc == expected_crc;
}