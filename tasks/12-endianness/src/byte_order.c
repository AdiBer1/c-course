#include "byte_order.h"

/* TODO: implement every function with shifts and ORs. */

uint16_t rd_u16_le(const uint8_t *p)
{
    (void)p;
    return 0;
}

uint16_t rd_u16_be(const uint8_t *p)
{
    (void)p;
    return 0;
}

uint32_t rd_u32_le(const uint8_t *p)
{
    (void)p;
    return 0;
}

uint32_t rd_u32_be(const uint8_t *p)
{
    (void)p;
    return 0;
}

void wr_u16_le(uint8_t *p, uint16_t v)
{
    (void)p;
    (void)v;
}

void wr_u16_be(uint8_t *p, uint16_t v)
{
    (void)p;
    (void)v;
}

void wr_u32_le(uint8_t *p, uint32_t v)
{
    (void)p;
    (void)v;
}

void wr_u32_be(uint8_t *p, uint32_t v)
{
    (void)p;
    (void)v;
}

int16_t rd_i16_be(const uint8_t *p)
{
    (void)p;
    return 0;
}

bool host_is_little_endian(void)
{
    return false;
}
