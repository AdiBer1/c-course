/*
 * hal_clock.h - HAL interface for a millisecond time base.
 *
 * On a microcontroller now_ms() would read a counter incremented by a timer
 * interrupt (like Arduino's millis()). It is a uint32_t and WRAPS AROUND to 0
 * after about 49.7 days, so always compare times like this:
 *
 *     if ((uint32_t)(now - start) >= duration) { ... }   // correct, wrap-safe
 *     if (now >= start + duration) { ... }               // WRONG near the wrap
 */
#ifndef HAL_CLOCK_H
#define HAL_CLOCK_H

#include <stdint.h>

typedef struct hal_clock {
    /* Milliseconds since some start point. Wraps around at 2^32. */
    uint32_t (*now_ms)(void *ctx);
    void *ctx;
} hal_clock_t;

#endif /* HAL_CLOCK_H */
