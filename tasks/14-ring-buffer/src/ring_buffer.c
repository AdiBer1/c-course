#include "ring_buffer.h"

rb_status_t rb_init(ring_buffer_t *rb, uint8_t *storage, size_t size)
{
    if (!rb || !storage) return RB_ERR_NULL;
    if (size == 0) return RB_ERR_SIZE;
    rb->buf   = storage;
    rb->size  = size;
    rb->head  = 0;
    rb->tail  = 0;
    rb->count = 0;
    return RB_OK;
}

rb_status_t rb_push(ring_buffer_t *rb, uint8_t byte)
{
    if (!rb) return RB_ERR_NULL;
    if (rb->count == rb->size) return RB_ERR_FULL;
    rb->buf[rb->head] = byte;
    rb->head = (rb->head + 1) % rb->size;
    rb->count++;
    return RB_OK;
}

rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *out)
{
    if (!rb || !out) return RB_ERR_NULL;
    if (rb->count == 0) return RB_ERR_EMPTY;
    *out = rb->buf[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    rb->count--;
    return RB_OK;
}

rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *out)
{
    if (!rb || !out) return RB_ERR_NULL;
    if (rb->count == 0) return RB_ERR_EMPTY;
    *out = rb->buf[rb->tail];
    return RB_OK;
}

size_t rb_count(const ring_buffer_t *rb)  { return rb ? rb->count : 0; }
size_t rb_free(const ring_buffer_t *rb)   { return rb ? rb->size - rb->count : 0; }
bool rb_is_empty(const ring_buffer_t *rb) { return rb ? rb->count == 0 : true; }
bool rb_is_full(const ring_buffer_t *rb)  { return rb ? rb->count == rb->size : false; }

void rb_clear(ring_buffer_t *rb)
{
    if (!rb) return;
    rb->head = rb->tail = rb->count = 0;
}

size_t rb_write(ring_buffer_t *rb, const uint8_t *data, size_t len)
{
    if (!rb || !data) return 0;
    size_t n = 0;
    while (n < len && rb_push(rb, data[n]) == RB_OK) n++;
    return n;
}

size_t rb_read(ring_buffer_t *rb, uint8_t *dst, size_t len)
{
    if (!rb || !dst) return 0;
    size_t n = 0;
    while (n < len && rb_pop(rb, &dst[n]) == RB_OK) n++;
    return n;
}
