#include "ring_buffer.h"

rb_status_t rb_init(ring_buffer_t *rb, uint8_t *storage, size_t size)
{
    if (rb == NULL || storage == NULL) {
        return RB_ERR_NULL;
    }
    if (size == 0) {
        return RB_ERR_SIZE;
    }

    rb->count = 0;
    rb->head = 0;
    rb->tail = 0;
    rb->size = size;
    rb->buf = storage;
    return RB_OK;
}

rb_status_t rb_push(ring_buffer_t *rb, uint8_t byte)
{
    if (rb == NULL) {
        return RB_ERR_NULL;
    }
    if (rb->count == rb->size) {
        return RB_ERR_FULL;
    }
    rb->buf[rb->head] = byte;
    rb->head = (rb->head + 1) % rb->size;
    rb->count++;
    return RB_OK;
}

rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL) {
        return RB_ERR_NULL;
    }
    if (rb->count == 0) {
        return RB_ERR_EMPTY;
    }
    *out = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    rb->count--;
    return RB_OK;
}

rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL) {
        return RB_ERR_NULL;
    }
    if (rb->count == 0) {
        return RB_ERR_EMPTY;
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
    if (rb == NULL) {
        return true;
    }
    return rb->count == 0;
}

bool rb_is_full(const ring_buffer_t *rb)
{
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
    rb->count = 0;
    rb->head = 0;
    rb->tail = 0;
}

size_t rb_write(ring_buffer_t *rb, const uint8_t *data, size_t len)
{
    if (rb == NULL || data == NULL) {
        return 0;
    }
    size_t i = 0;
    for (; i < len; i++) {
        if (rb_push(rb, data[i]) == RB_ERR_FULL) {
            return i;
        }
    }
    return i;
}

size_t rb_read(ring_buffer_t *rb, uint8_t *dst, size_t len)
{
    if (rb == NULL || dst == NULL) {
        return 0;
    }
    size_t i = 0;
    for (; i < len; i++) {
        if (rb_pop(rb, &dst[i]) == RB_ERR_EMPTY) {
            return i;
        }
    }
    return i;
}