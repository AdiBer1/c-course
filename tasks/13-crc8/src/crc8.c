#include "crc8.h"

uint8_t crc8_update(uint8_t crc, uint8_t byte)
{
    crc ^= byte;

    for (int i = 0; i < 8; i++)
    {
        if ((crc & 0x80u) != 0u)
        {
            crc = (uint8_t)(((unsigned int)crc << 1u) ^
                            (unsigned int)CRC8_POLY);
        }
        else
        {
            crc = (uint8_t)((unsigned int)crc << 1u);
        }
    }

    return crc;
}

uint8_t crc8(const uint8_t *data, size_t len)
{
    if (data == NULL)
    {
        return CRC8_INIT;
    }

    uint8_t crc = CRC8_INIT;

    for (size_t i = 0; i < len; i++)
    {
        crc = crc8_update(crc, data[i]);
    }

    return crc;
}

bool crc8_frame_ok(const uint8_t *frame, size_t len)
{
    if (frame == NULL || len < 2)
    {
        return false;
    }

    uint8_t calculated_crc = crc8(frame, len - 1u);

    return calculated_crc == frame[len - 1u];
}
