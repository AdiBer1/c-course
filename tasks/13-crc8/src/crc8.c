#include "crc8.h"

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    crc ^= byte; /* XOR the new byte into the register */

    for (int bit = 0; bit < 8; bit++) {
        /* Test the top bit BEFORE shifting: it is the bit that falls out. */
        if ((crc & 0x80u) != 0u) {
            /* crc << 1 is computed as int (integer promotion); the cast keeps
             * the low 8 bits, which is exactly "keep crc 8 bits wide". */
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
        return false; /* no room for at least one data byte plus the CRC */
    }

    /* The last byte is the CRC of all the bytes BEFORE it, so it is not
     * included in the computation. */
    return crc8(frame, len - 1u) == frame[len - 1u];
}