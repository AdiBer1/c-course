#include "ring_buffer.h"

rb_status_t rb_init(ring_buffer_t *rb, uint8_t *storage, size_t size)
{
    if (rb == NULL || storage == NULL) {
        return RB_ERR_NULL;
    }
    if (size == 0u) {
        return RB_ERR_SIZE;
    }

    rb->buf = storage;
    rb->size = size;
    rb->head = 0u;
    rb->tail = 0u;
    rb->count = 0u;

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
    rb->head++;
    if (rb->head == rb->size) {
        rb->head = 0u;
    }
    rb->count++;

    return RB_OK;
}

rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL) {
        return RB_ERR_NULL;
    }
    if (rb->count == 0u) {
        return RB_ERR_EMPTY;
    }

    *out = rb->buf[rb->tail];
    rb->tail++;
    if (rb->tail == rb->size) {
        rb->tail = 0u;
    }
    rb->count--;

    return RB_OK;
}

rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL) {
        return RB_ERR_NULL;
    }
    if (rb->count == 0u) {
        return RB_ERR_EMPTY;
    }

    *out = rb->buf[rb->tail];

    return RB_OK;
}

size_t rb_count(const ring_buffer_t *rb)
{
    if (rb == NULL) {
        return 0u;
    }
    return rb->count;
}

size_t rb_free(const ring_buffer_t *rb)
{
    if (rb == NULL) {
        return 0u;
    }
    return rb->size - rb->count;
}

bool rb_is_empty(const ring_buffer_t *rb)
{
    if (rb == NULL) {
        return true;
    }
    return rb->count == 0u;
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
    rb->head = 0u;
    rb->tail = 0u;
    rb->count = 0u;
}

size_t rb_write(ring_buffer_t *rb, const uint8_t *data, size_t len)
{
    size_t written = 0u;

    if (rb == NULL || data == NULL) {
        return 0u;
    }

    while (written < len) {
        if (rb_push(rb, data[written]) != RB_OK) {
            break;
        }
        written++;
    }

    return written;
}

size_t rb_read(ring_buffer_t *rb, uint8_t *dst, size_t len)
{
    size_t done = 0u;

    if (rb == NULL || dst == NULL) {
        return 0u;
    }

    while (done < len) {
        if (rb_pop(rb, &dst[done]) != RB_OK) {
            break;
        }
        done++;
    }

    return done;
}