/*
 * sensor_log.h - keeps the last few sensor readings in a fixed array.
 *
 * The log lives in STATIC memory: one array that exists for the whole run
 * of the program. There is no malloc anywhere (from stage 3 on it is not
 * allowed), which is how most firmware stores data.
 *
 * Do not change this file. The bugs are all in sensor_log.c.
 */
#ifndef SENSOR_LOG_H
#define SENSOR_LOG_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LOG_CAPACITY 8

/* Empty the log. */
void log_init(void);

/* Append a reading. Returns false (and stores nothing) when the log is full. */
bool log_add(int16_t value);

/* Number of readings stored (0..LOG_CAPACITY). */
size_t log_count(void);

/* Reading number `index` (0 = oldest). Returns 0 if index >= log_count(). */
int16_t log_get(size_t index);

/* Average of all readings, rounded toward zero. 0 when the log is empty. */
int16_t log_average(void);

/*
 * Copy the newest k readings into dst, oldest of them first, where
 * k = the smallest of n, log_count() and dst_len. Returns k.
 * Example: log holds 1 2 3 4 5; log_copy_last(dst, 8, 3) writes 3 4 5, returns 3.
 */
size_t log_copy_last(int16_t *dst, size_t dst_len, size_t n);

/*
 * A name for this log: "sensor-log/8" (8 = LOG_CAPACITY).
 * The returned string stays valid for the whole run of the program, and
 * every call returns the same pointer.
 */
const char *log_label(void);

#endif /* SENSOR_LOG_H */
