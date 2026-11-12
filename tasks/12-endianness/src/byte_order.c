#include "byte_order.h"

/*
 * Every value is built or split byte by byte with shifts and ORs, so the
 * result never depends on this CPU's own byte order, and the buffer may
 * have any alignment (only single bytes are ever accessed).
 *
 * Each byte is cast to uint32_t BEFORE shifting: a uint8_t is otherwise
 * promoted to a signed int, and 0xFF << 24 does not fit in an int
 * (signed overflow, undefined behaviour).
 */

/* ------------------------------------------------------------------ read */

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

/* ----------------------------------------------------------------- write */
/* Each byte: shift the wanted 8 bits down to the bottom, mask with 0xFF,
 * and cast explicitly to uint8_t. */

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

/* ---------------------------------------------------------------- signed */

int16_t rd_i16_be(const uint8_t *p)
{
    /*
     * Read the raw 16 bits, then interpret them as two's complement:
     * if the top bit is set, the value is raw - 65536.
     * FF 38 -> 0xFF38 = 65336 -> 65336 - 65536 = -200.
     */
    uint16_t raw = rd_u16_be(p);
    int32_t value = (int32_t)raw;
    if (value >= 0x8000) {
        value -= 0x10000;
    }
    return (int16_t)value;
}

/* ------------------------------------------------------------------ host */

bool host_is_little_endian(void)
{
    /*
     * Store 1 in a uint32_t and look at its FIRST byte in memory
     * (reading any object through unsigned char * is always allowed).
     * Little-endian stores the least significant byte first: 01 00 00 00.
     * Big-endian stores it last: 00 00 00 01.
     */
    const uint32_t one = 1u;
    const unsigned char *bytes = (const unsigned char *)&one;
    return bytes[0] == 1u;
}