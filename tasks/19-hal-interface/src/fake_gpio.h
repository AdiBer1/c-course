/*
 * fake_gpio.h - YOUR OWN implementation of the hal_gpio_t interface.
 *
 * The other side of the interface: instead of using a GPIO, you provide one.
 * This fake keeps a log of the first writes (handy to check the exact
 * sequence a driver produced) and returns test-chosen levels on read.
 *
 * You write two static functions in fake_gpio.c that match the function
 * pointer types in hal_gpio_t, and fake_gpio_init() puts their addresses
 * (and the fake itself as ctx) into the hal_gpio_t.
 *
 * Do not change this file.
 */
#ifndef FAKE_GPIO_H
#define FAKE_GPIO_H

#include <stddef.h>
#include <stdint.h>

#include "hal_gpio.h"

#define FAKE_GPIO_LOG_SIZE 8u
#define FAKE_GPIO_PINS     8u

typedef struct {
    uint8_t pin;
    gpio_level_t level;
} fake_gpio_write_t;

typedef struct {
    fake_gpio_write_t log[FAKE_GPIO_LOG_SIZE]; /* the FIRST writes, in order */
    size_t log_count;                          /* entries used in log (max FAKE_GPIO_LOG_SIZE) */
    uint32_t total_writes;                     /* every write, also those that did not fit */
    gpio_level_t input_level[FAKE_GPIO_PINS];  /* what read() returns for each pin */
    uint32_t reads;                            /* number of read() calls */
} fake_gpio_t;

/*
 * Clear the fake (empty log, counters 0, all inputs LOW) and fill *hal so
 * that hal->write and hal->read go to this fake (hal->ctx = fake).
 * Does nothing if fake or hal is NULL.
 *
 * Behaviour of the installed functions:
 *   write: append {pin, level} to the log if there is room; always count it
 *   read:  count it; return input_level[pin], or GPIO_LOW if pin >= FAKE_GPIO_PINS
 */
void fake_gpio_init(fake_gpio_t *fake, hal_gpio_t *hal);

#endif /* FAKE_GPIO_H */
