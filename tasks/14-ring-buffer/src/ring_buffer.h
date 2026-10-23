/*
 * ring_buffer.h - a FIFO byte queue over a fixed array (circular buffer).
 *
 * The workhorse data structure of firmware: a UART interrupt pushes received
 * bytes, the main loop pops them; a logger queues messages until the radio is
 * free. The array never moves; two indices chase each other around it:
 *
 *        tail (next to pop)        head (next free slot)
 *            v                         v
 *     [ . . A B C D . . . . . . . . . . ]   count = 4
 *
 * When an index reaches the end of the array it wraps back to 0.
 *
 * The caller provides the storage, so one implementation serves buffers of
 * any size, all statically allocated:
 *
 *     static uint8_t rx_storage[64];
 *     static ring_buffer_t rx;
 *     rb_init(&rx, rx_storage, sizeof rx_storage);
 *
 * The struct is visible so it can be allocated statically, but only these
 * functions may touch its fields.
 *
 * Constraints: no malloc. Every function must be O(1) except rb_write/rb_read
 * which are O(len).
 *
 * Do not change this file.
 */
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    RB_OK = 0,
    RB_ERR_NULL,  /* rb (or another required pointer) is NULL */
    RB_ERR_SIZE,  /* rb_init: size is 0 */
    RB_ERR_FULL,  /* push into a full buffer */
    RB_ERR_EMPTY  /* pop/peek from an empty buffer */
} rb_status_t;

typedef struct {
    uint8_t *buf;  /* caller's storage */
    size_t size;   /* capacity in bytes: ALL `size` bytes can be used */
    size_t head;   /* index where the next byte is written */
    size_t tail;   /* index of the oldest byte */
    size_t count;  /* number of bytes stored */
} ring_buffer_t;

/* Set up an empty buffer over storage[0..size-1]. */
rb_status_t rb_init(ring_buffer_t *rb, uint8_t *storage, size_t size);

/* Append one byte. RB_ERR_FULL if there is no room (the byte is dropped). */
rb_status_t rb_push(ring_buffer_t *rb, uint8_t byte);

/* Remove the oldest byte into *out. RB_ERR_EMPTY if there is none (out unchanged). */
rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *out);

/* Like rb_pop but leaves the byte in the buffer. */
rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *out);

/* Number of bytes stored / free. NULL -> 0. */
size_t rb_count(const ring_buffer_t *rb);
size_t rb_free(const ring_buffer_t *rb);

/* NULL counts as empty and as not full. */
bool rb_is_empty(const ring_buffer_t *rb);
bool rb_is_full(const ring_buffer_t *rb);

/* Drop everything. */
void rb_clear(ring_buffer_t *rb);

/*
 * Push up to len bytes from data, stopping when the buffer is full.
 * Returns how many were pushed (0 for NULL arguments).
 */
size_t rb_write(ring_buffer_t *rb, const uint8_t *data, size_t len);

/*
 * Pop up to len bytes into dst, oldest first, stopping when the buffer is
 * empty. Returns how many were popped (0 for NULL arguments).
 */
size_t rb_read(ring_buffer_t *rb, uint8_t *dst, size_t len);

#endif /* RING_BUFFER_H */
