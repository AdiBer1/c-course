/*
 * status_reg.h - decode the 32-bit STATUS register of a (fake) sensor chip.
 *
 * Hardware reports its state in registers: one 32-bit word with several
 * flags and small numbers packed into bit fields. Driver code turns that
 * word into a readable struct, and back.
 *
 * STATUS register layout:
 *
 *   31   28 27              16 15          8 7    4   3    2     1     0
 *  +-------+------------------+-------------+------+-----+----+-----+-----+
 *  |VERSION|   TEMPERATURE    | FIFO_LEVEL  | MODE | res | OVR| ERR | RDY |
 *  +-------+------------------+-------------+------+-----+----+-----+-----+
 *
 *   bit 0       READY       1 = a new measurement is available
 *   bit 1       ERROR       1 = the chip detected a fault
 *   bit 2       OVERRUN     1 = data was lost (FIFO overflowed)
 *   bit 3       reserved    may read as 0 or 1: ignore it, write 0
 *   bits 4..7   MODE        0 IDLE, 1 MEASURE, 2 CALIBRATE, 3 SLEEP,
 *                           4..15 are invalid
 *   bits 8..15  FIFO_LEVEL  samples waiting, 0..255
 *   bits 16..27 TEMPERATURE 12-bit TWO'S COMPLEMENT, 1 LSB = 1/16 degree C
 *                           (0x190 = 400 = +25.0 C, 0xFFF = -1 = -0.0625 C)
 *   bits 28..31 VERSION     chip revision, 0..15
 *
 * On a real microcontroller the register is at a fixed address and is read
 * through a volatile pointer, for example:
 *
 *     #define SENSOR_STATUS (*(const volatile uint32_t *)0x40021000u)
 *     status_t s = status_decode(SENSOR_STATUS);
 *
 * The tests use an ordinary variable instead of a real address.
 *
 * Do not change this file.
 */
#ifndef STATUS_REG_H
#define STATUS_REG_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    DEV_MODE_IDLE = 0,
    DEV_MODE_MEASURE = 1,
    DEV_MODE_CALIBRATE = 2,
    DEV_MODE_SLEEP = 3,
    DEV_MODE_INVALID = 4 /* register said 4..15 */
} dev_mode_t;

typedef struct {
    bool ready;
    bool error;
    bool overrun;
    dev_mode_t mode;
    uint8_t fifo_level;
    int16_t temp_sixteenths; /* -2048 .. 2047, in 1/16 degree C */
    uint8_t version;
} status_t;

/* Decode a raw register value. */
status_t status_decode(uint32_t reg);

/*
 * Build the raw register value from a status (the inverse of status_decode).
 *   - mode DEV_MODE_INVALID (or any value that is not a valid mode) -> MODE field 0xF
 *   - temp_sixteenths outside -2048..2047 is clamped to that range
 *   - fifo_level and version use their bits as-is (version: low 4 bits only)
 *   - the reserved bit is 0
 * Returns 0 if s is NULL.
 */
uint32_t status_encode(const status_t *s);

/*
 * Read the register at `reg` exactly ONCE and decode it. Reading it twice
 * could see two different values, because hardware changes it at any time.
 * If reg is NULL, behaves like status_decode(0).
 */
status_t status_read(const volatile uint32_t *reg);

/* Temperature in whole degrees C, rounded to nearest (halves away from zero). NULL -> 0. */
int16_t status_temp_celsius(const status_t *s);

#endif /* STATUS_REG_H */
