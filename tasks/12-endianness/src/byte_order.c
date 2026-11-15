#include "byte_order.h"

/* TODO: implement every function with shifts and ORs. */

uint16_t rd_u16_le(const uint8_t *p)
{
    return (uint16_t)(
        (uint16_t)p[0] |
        ((uint16_t)p[1] << 8)
    );
}

uint16_t rd_u16_be(const uint8_t *p)
{
    return (uint16_t)(
        ((uint16_t)p[0] << 8) |
        (uint16_t)p[1]
    );
}

uint32_t rd_u32_le(const uint8_t *p)
{
    return (uint32_t)(
        (uint32_t)p[0] |
        ((uint32_t)p[1] << 8) |
        ((uint32_t)p[2] << 16) |
        ((uint32_t)p[3] << 24)
    );
}

uint32_t rd_u32_be(const uint8_t *p)
{
    return (uint32_t)(
        ((uint32_t)p[0] << 24) |
        ((uint32_t)p[1] << 16) |
        ((uint32_t)p[2] << 8) |
        (uint32_t)p[3]
    );
}

void wr_u16_le(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)(v & 0xFF);
    p[1] = (uint8_t)((v >> 8) & 0xFF);
}

void wr_u16_be(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)((v >> 8) & 0xFF);
    p[1] = (uint8_t)(v & 0xFF);
}

void wr_u32_le(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v & 0xFF);
    p[1] = (uint8_t)((v >> 8) & 0xFF);
    p[2] = (uint8_t)((v >> 16) & 0xFF);
    p[3] = (uint8_t)((v >> 24) & 0xFF);
}

void wr_u32_be(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)((v >> 24) & 0xFF);
    p[1] = (uint8_t)((v >> 16) & 0xFF);
    p[2] = (uint8_t)((v >> 8) & 0xFF);
    p[3] = (uint8_t)(v & 0xFF);
}

int16_t rd_i16_be(const uint8_t *p)
{
    uint16_t value = rd_u16_be(p);

    if (value & 0x8000) {
        return (int16_t)(value - 65536);
    }

    return (int16_t)value;
}

bool host_is_little_endian(void)
{
    const uint16_t value = 1;
    const uint8_t *p = (const uint8_t *)&value;

    return p[0] == 1;
}