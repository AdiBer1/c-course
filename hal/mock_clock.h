/*
 * mock_clock.h - a fake millisecond clock for tests (implements hal_clock_t).
 *
 * Time only moves when the test says so. Code that busy-waits for time to
 * pass will therefore hang forever, and the test hits its 5-second limit.
 */
#ifndef MOCK_CLOCK_H
#define MOCK_CLOCK_H

#include <stdint.h>

#include "hal_clock.h"

typedef struct {
    uint32_t now;
    uint32_t reads; /* how many times now_ms() was called */
} mock_clock_t;

/* Start the clock at `start_ms` and point `hal` at it. */
void mock_clock_init(mock_clock_t *mock, hal_clock_t *hal, uint32_t start_ms);

/* Move time forward (wraps around like the real counter). */
void mock_clock_advance(mock_clock_t *mock, uint32_t ms);

#endif /* MOCK_CLOCK_H */
