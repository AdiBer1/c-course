#include "ring_buffer.h"

/* Moves an index one slot forward, wrapping from the last slot back to 0.
 * O(1), and avoids the cost of a modulo operation. */
static size_t advance(const ring_buffer_t *rb, size_t index)
{
    index++;
    if (index == rb->size) {
        index = 0;
    }
    return index;
}

rb_status_t rb_init(ring_buffer_t *rb, uint8_t *storage, size_t size)
{
    if (rb == NULL || storage == NULL) {
        return RB_ERR_NULL;
    }
    if (size == 0) {
        return RB_ERR_SIZE;
    }

    rb->buf = storage;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    return RB_OK;
}

rb_status_t rb_push(ring_buffer_t *rb, uint8_t byte)
{
    if (rb == NULL) {
        return RB_ERR_NULL;
    }
    if (rb_is_full(rb)) {
        return RB_ERR_FULL;
    }

    rb->buf[rb->head] = byte;
    rb->head = advance(rb, rb->head);
    rb->count++;
    return RB_OK;
}

rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL) {
        return RB_ERR_NULL;
    }
    if (rb->count == 0) {
        return RB_ERR_EMPTY; /* *out is left unchanged */
    }

    *out = rb->buf[rb->tail];
    rb->tail = advance(rb, rb->tail);
    rb->count--;
    return RB_OK;
}

rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL) {
        return RB_ERR_NULL;
    }
    if (rb->count == 0) {
        return RB_ERR_EMPTY; /* *out is left unchanged */
    }

    *out = rb->buf[rb->tail];
    return RB_OK;
}

size_t rb_count(const ring_buffer_t *rb)
{
    if (rb == NULL) {
        return 0;
    }
    return rb->count;
}

size_t rb_free(const ring_buffer_t *rb)
{
    if (rb == NULL) {
        return 0;
    }
    return rb->size - rb->count;
}

bool rb_is_empty(const ring_buffer_t *rb)
{
    /* NULL counts as empty */
    if (rb == NULL) {
        return true;
    }
    return rb->count == 0;
}

bool rb_is_full(const ring_buffer_t *rb)
{
    /* NULL counts as not full */
    if (rb == NULL) {
        return false;
    }
    return rb->count == rb->size;
}

void rb_clear(ring_buffer_t *rb)
{
    if (rb == NULL) {
        return;
    }
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

size_t rb_write(ring_buffer_t *rb, const uint8_t *data, size_t len)
{
    size_t written = 0;

    if (rb == NULL || data == NULL) {
        return 0;
    }

    /* rb_push fails only when the buffer is full, which ends the loop */
    while (written < len && rb_push(rb, data[written]) == RB_OK) {
        written++;
    }
    return written;
}

size_t rb_read(ring_buffer_t *rb, uint8_t *dst, size_t len)
{
    size_t popped = 0;

    if (rb == NULL || dst == NULL) {
        return 0;
    }

    /* rb_pop fails only when the buffer is empty, which ends the loop */
    while (popped < len && rb_pop(rb, &dst[popped]) == RB_OK) {
        popped++;
    }
    return popped;
}