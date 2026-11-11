#include "byte_order.h"

uint16_t rd_u16_le(const uint8_t *p)
{
    return ((uint16_t)p[0])
         | ((uint16_t)p[1] << 8);
}

uint16_t rd_u16_be(const uint8_t *p)
{
    return ((uint16_t)p[0] << 8)
         | ((uint16_t)p[1]);
}

uint32_t rd_u32_le(const uint8_t *p)
{
    return ((uint32_t)p[0])
         | ((uint32_t)p[1] << 8)
         | ((uint32_t)p[2] << 16)
         | ((uint32_t)p[3] << 24);
}

uint32_t rd_u32_be(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24)
         | ((uint32_t)p[1] << 16)
         | ((uint32_t)p[2] << 8)
         | ((uint32_t)p[3]);
}

void wr_u16_le(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
}

void wr_u16_be(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)(v >> 8);
    p[1] = (uint8_t)v;
}

void wr_u32_le(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)v;
    p[1] = (uint8_t)(v >> 8);
    p[2] = (uint8_t)(v >> 16);
    p[3] = (uint8_t)(v >> 24);
}

void wr_u32_be(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v >> 24);
    p[1] = (uint8_t)(v >> 16);
    p[2] = (uint8_t)(v >> 8);
    p[3] = (uint8_t)v;
}

int16_t rd_i16_be(const uint8_t *p)
{
    uint16_t raw = rd_u16_be(p);
    return (int16_t)raw;
}

bool host_is_little_endian(void)
{
    uint16_t value = 1;
    const uint8_t *bytes = (const uint8_t *)&value;

    return bytes[0] == 1;
}