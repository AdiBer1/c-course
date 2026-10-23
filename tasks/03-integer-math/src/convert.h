/*
 * convert.h - unit conversions with integer-only math.
 *
 * Many microcontrollers have no floating-point unit; float math there is
 * slow and big. So sensor values are kept as integers in small units
 * ("tenths of a degree", "millivolts") and converted with integer math.
 *
 * All results are ROUNDED TO THE NEAREST integer. Plain C integer division
 * truncates toward zero (7 / 2 == 3, -7 / 2 == -3), so you must round
 * yourself, for negative values too.
 *
 * Constraints: no float, no double, no math library.
 *
 * Do not change this file.
 */
#ifndef CONVERT_H
#define CONVERT_H

#include <stdint.h>

/*
 * Tenths of a degree Celsius -> tenths of a degree Fahrenheit.
 *     F = C * 9 / 5 + 32
 * Examples: 0 -> 320 (32.0 F), 1000 -> 2120, -400 -> -400,
 *           1 (0.1 C = 32.18 F) -> 322, -1 (-0.1 C = 31.82 F) -> 318.
 * Valid input range: -100000 .. 100000.
 */
int32_t c10_to_f10(int32_t c10);

/*
 * Tenths of a degree Fahrenheit -> tenths of a degree Celsius.
 *     C = (F - 32) * 5 / 9
 * Examples: 320 -> 0, 2120 -> 1000, 0 (0.0 F = -17.78 C) -> -178.
 * Valid input range: -100000 .. 100000.
 */
int32_t f10_to_c10(int32_t f10);

/*
 * Raw reading of a 12-bit ADC (0..4095) -> millivolts, with a 3300 mV
 * reference: 0 -> 0 mV, 4095 -> 3300 mV, 2048 -> 1650 mV (1650.4 rounded).
 *     mV = raw * 3300 / 4095
 * Raw values above 4095 are impossible on real hardware: treat them as 4095.
 */
uint16_t adc_to_mv(uint16_t raw);

#endif /* CONVERT_H */
