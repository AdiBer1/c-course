#include "byte_order.h"

uint16_t rd_u16_le(const uint8_t *p)
{
    return (uint16_t)((uint32_t)p[0] | ((uint32_t)p[1] << 8));
}

uint16_t rd_u16_be(const uint8_t *p)
{
    return (uint16_t)(((uint32_t)p[0] << 8) | (uint32_t)p[1]);
}

uint32_t rd_u32_le(const uint8_t *p)
{
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

uint32_t rd_u32_be(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

void wr_u16_le(uint8_t *p, uint16_t v)
{
    uint32_t x = v;
    p[0] = (uint8_t)(x & 0xFFu);
    p[1] = (uint8_t)((x >> 8) & 0xFFu);
}

void wr_u16_be(uint8_t *p, uint16_t v)
{
    uint32_t x = v;
    p[0] = (uint8_t)((x >> 8) & 0xFFu);
    p[1] = (uint8_t)(x & 0xFFu);
}

void wr_u32_le(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
    p[2] = (uint8_t)((v >> 16) & 0xFFu);
    p[3] = (uint8_t)((v >> 24) & 0xFFu);
}

void wr_u32_be(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)((v >> 24) & 0xFFu);
    p[1] = (uint8_t)((v >> 16) & 0xFFu);
    p[2] = (uint8_t)((v >> 8) & 0xFFu);
    p[3] = (uint8_t)(v & 0xFFu);
}

int16_t rd_i16_be(const uint8_t *p)
{
    int32_t v = (int32_t)rd_u16_be(p);
    if (v >= 32768) {
        v = v - 65536;
    }
    return (int16_t)v;
}

bool host_is_little_endian(void)
{
    uint16_t x = 1u;
    const uint8_t *b = (const uint8_t *)&x;
    return b[0] == 1u;
}