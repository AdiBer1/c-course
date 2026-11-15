#include "byte_order.h"

/*
 * Every byte is cast to uint32_t BEFORE shifting: a plain uint8_t would be
 * promoted to (signed) int, and 0xFF << 24 overflows int (undefined behaviour).
 */

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
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

uint32_t rd_u32_be(const uint8_t *p)
{
    return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) |
           (uint32_t)p[3];
}

void wr_u16_le(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)(v & 0xFFu);
    p[1] = (uint8_t)((v >> 8) & 0xFFu);
}

void wr_u16_be(uint8_t *p, uint16_t v)
{
    p[0] = (uint8_t)((v >> 8) & 0xFFu);
    p[1] = (uint8_t)(v & 0xFFu);
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
    /* Read the 16 bits unsigned, then interpret them as two's complement. */
    int32_t raw = (int32_t)rd_u16_be(p);
    if (raw >= 0x8000) {
        raw -= 0x10000;
    }
    return (int16_t)raw;
}

bool host_is_little_endian(void)
{
    /* Look at the first byte of a known 16-bit value in memory.
       Inspecting any object through an unsigned char pointer is allowed. */
    const uint16_t probe = 0x0001u;
    const unsigned char *first = (const unsigned char *)&probe;
    return *first == 0x01u;
}
