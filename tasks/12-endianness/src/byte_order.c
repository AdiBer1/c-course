
#include "byte_order.h"

uint16_t rd_u16_le(const uint8_t *p)
{
    return (uint16_t)(
        (uint32_t)p[0] |
        ((uint32_t)p[1] << 8u)
    );
}

uint16_t rd_u16_be(const uint8_t *p)
{
    return (uint16_t)(
        ((uint32_t)p[0] << 8u) |
        (uint32_t)p[1]
    );
}

uint32_t rd_u32_le(const uint8_t *p)
{
    return (uint32_t)p[0] |
           ((uint32_t)p[1] << 8u) |
           ((uint32_t)p[2] << 16u) |
           ((uint32_t)p[3] << 24u);
}

uint32_t rd_u32_be(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24u) |
           ((uint32_t)p[1] << 16u) |
           ((uint32_t)p[2] << 8u) |
           (uint32_t)p[3];
}

void wr_u16_le(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8u);
}

void wr_u16_be(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)(v >> 8u);
    p[1] = (uint8_t)v;
}

void wr_u32_le(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8u);
    p[2] = (uint8_t)(v >> 16u);
    p[3] = (uint8_t)(v >> 24u);
}

void wr_u32_be(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v >> 24u);
    p[1] = (uint8_t)(v >> 16u);
    p[2] = (uint8_t)(v >> 8u);
    p[3] = (uint8_t)v;
}

int16_t rd_i16_be(const uint8_t *p)
{
    uint16_t raw = rd_u16_be(p);

    int32_t value = (raw & 0x8000u) != 0u
                        ? (int32_t)raw - 65536
                        : (int32_t)raw;

    return (int16_t)value;
}

bool host_is_little_endian(void)
{
    const uint16_t value = 1u;
    const uint8_t *bytes = (const uint8_t *)&value;

    return bytes[0] == 1u;
}
