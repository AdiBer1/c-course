/*
 * hal_gpio.h - Hardware Abstraction Layer interface for digital pins (GPIO).
 *
 * Driver code (an LED, a button, a traffic light) never touches hardware
 * registers directly. It only calls the two functions in this struct.
 * Who fills the struct decides what "a pin" really is:
 *   - on a microcontroller: functions that write the port registers
 *   - in the tests:         a mock that just records what happened
 * The driver code is the same in both cases. That is the whole point.
 *
 * `ctx` is passed back to every call so one implementation can serve several
 * instances (e.g. two GPIO ports, or two independent mocks).
 */
#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} gpio_level_t;

typedef struct hal_gpio {
    /* Drive output pin `pin` to `level`. */
    void (*write)(void *ctx, uint8_t pin, gpio_level_t level);
    /* Read the current level of input pin `pin`. */
    gpio_level_t (*read)(void *ctx, uint8_t pin);
    /* Implementation data, passed unchanged to write() and read(). */
    void *ctx;
} hal_gpio_t;

#endif /* HAL_GPIO_H */
