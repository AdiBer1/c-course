#include "byte_order.h"

/* TODO: implement every function with shifts and ORs. */

uint16_t rd_u16_le(const uint8_t *p)
{
    return (uint16_t)((uint16_t)p[0] | (uint16_t)p[1] << 8);
}

uint16_t rd_u16_be(const uint8_t *p)
{
    return (uint16_t)((uint16_t)p[1] | (uint16_t)p[0] << 8);
}

uint32_t rd_u32_le(const uint8_t *p)
{
    return (uint32_t)((uint32_t)p[3] << 24 | (uint32_t)p[2] << 16 | (uint32_t)p[1] << 8
                      | (uint32_t)p[0]);
}

uint32_t rd_u32_be(const uint8_t *p)
{
    return (uint32_t)((uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8
                      | (uint32_t)p[3]);
}

void wr_u16_le(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)(v >> 8 & 0xFF);
}

void wr_u16_be(uint8_t *p, uint16_t v)
{
    p[1] = (uint8_t)(v & 0xFFu);
    p[0] = (uint8_t)(v >> 8 & 0xFF);
}

void wr_u32_le(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v & 0xFFFFu);
    p[1] = (uint8_t)(v >> 8 & 0xFFu);
    p[2] = (uint8_t)(v >> 16 & 0xFFu);
    p[3] = (uint8_t)(v >> 24 & 0xFFu);
}

void wr_u32_be(uint8_t *p, uint32_t v)
{
    p[3] = (uint8_t)(v & 0xFFu);
    p[2] = (uint8_t)(v >> 8 & 0xFFu);
    p[1] = (uint8_t)(v >> 16 & 0xFFu);
    p[0] = (uint8_t)(v >> 24 & 0xFFu);
}

int16_t rd_i16_be(const uint8_t *p)
{
    uint16_t u = rd_u16_be(p);
    if (u < 0x8000u) {
        return (int16_t)u;
    }
    return (int16_t)((int32_t)u - 0x10000);
}
bool host_is_little_endian(void)
{
    uint16_t x = 1;
    const uint8_t *b = (const uint8_t *)&x;

    return b[0] == 1;
}
