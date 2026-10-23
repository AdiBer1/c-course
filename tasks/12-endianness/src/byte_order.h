/*
 * byte_order.h - read and write multi-byte integers in a byte buffer.
 *
 * A uint32_t 0x12345678 is four bytes. In which ORDER they are stored is the
 * "endianness":
 *     little-endian (x86, most ARM MCUs):  78 56 34 12
 *     big-endian    ("network order"):     12 34 56 78
 *
 * Sensor datasheets, CAN/Modbus/network protocols and file formats each
 * define their own byte order. Firmware must convert explicitly, byte by
 * byte, so the code works no matter what CPU it runs on.
 *
 * Constraints:
 *   - build values with shifts and ORs, byte by byte
 *   - no memcpy, no casting the buffer to (uint32_t *): the buffer may be
 *     unaligned, and the result would depend on the CPU's own byte order
 *   - no htons/htonl/ntohs/ntohl and no compiler built-ins
 *
 * The buffer pointer `p` must point to at least 2 (u16) or 4 (u32) bytes.
 * It may have any alignment.
 *
 * Do not change this file.
 */
#ifndef BYTE_ORDER_H
#define BYTE_ORDER_H

#include <stdbool.h>
#include <stdint.h>

uint16_t rd_u16_le(const uint8_t *p);
uint16_t rd_u16_be(const uint8_t *p);
uint32_t rd_u32_le(const uint8_t *p);
uint32_t rd_u32_be(const uint8_t *p);

void wr_u16_le(uint8_t *p, uint16_t v);
void wr_u16_be(uint8_t *p, uint16_t v);
void wr_u32_le(uint8_t *p, uint32_t v);
void wr_u32_be(uint8_t *p, uint32_t v);

/* A signed 16-bit big-endian value (two's complement): bytes FF 38 -> -200. */
int16_t rd_i16_be(const uint8_t *p);

/* true if THIS computer stores integers little-endian. Find out at run time. */
bool host_is_little_endian(void);

#endif /* BYTE_ORDER_H */
