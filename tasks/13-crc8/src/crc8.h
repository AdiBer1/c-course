/*
 * crc8.h - CRC-8 checksum (the variant used by SMBus).
 *
 * A CRC detects corrupted data: the sender appends a CRC byte computed over
 * the message; the receiver recomputes it and compares. Unlike a simple sum
 * it catches swapped bytes, all single-bit errors and all burst errors up to
 * 8 bits long. Sensirion sensors, SMBus, 1-Wire and many radio protocols use
 * a CRC-8.
 *
 * Parameters of this CRC-8 ("CRC-8/SMBUS"):
 *   polynomial 0x07 (x^8 + x^2 + x + 1), initial value 0x00,
 *   bits processed most-significant first, no final XOR.
 *   Check value: crc8("123456789", 9) == 0xF4.
 *
 * The bit-by-bit algorithm for one byte:
 *     crc ^= byte;
 *     repeat 8 times:
 *         if the top bit (0x80) of crc is set: crc = (crc << 1) ^ 0x07
 *         else:                                crc = crc << 1
 *     (keep crc 8 bits wide)
 *
 * Do not change this file.
 */
#ifndef CRC8_H
#define CRC8_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CRC8_POLY 0x07u
#define CRC8_INIT 0x00u

/* Feed one byte into a running CRC and return the new CRC (for streaming data). */
uint8_t crc8_update(uint8_t crc, uint8_t byte);

/* CRC of data[0..len-1], starting from CRC8_INIT. len == 0 -> CRC8_INIT. data == NULL -> CRC8_INIT. */
uint8_t crc8(const uint8_t *data, size_t len);

/*
 * Check a received frame whose LAST byte is the CRC of all bytes before it.
 * frame = {0x01, 0x02, crc8({0x01, 0x02})} -> true.
 * Returns false for NULL or len < 2 (no room for data plus CRC).
 */
bool crc8_frame_ok(const uint8_t *frame, size_t len);

#endif /* CRC8_H */
