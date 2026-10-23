#include "ring_buffer.h"

/* TODO: implement every function. */

rb_status_t rb_init(ring_buffer_t *rb, uint8_t *storage, size_t size)
{
    (void)rb;
    (void)storage;
    (void)size;
    return RB_OK;
}

rb_status_t rb_push(ring_buffer_t *rb, uint8_t byte)
{
    (void)rb;
    (void)byte;
    return RB_OK;
}

rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *out)
{
    (void)rb;
    (void)out;
    return RB_OK;
}

rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *out)
{
    (void)rb;
    (void)out;
    return RB_OK;
}

size_t rb_count(const ring_buffer_t *rb)
{
    (void)rb;
    return 0;
}

size_t rb_free(const ring_buffer_t *rb)
{
    (void)rb;
    return 0;
}

bool rb_is_empty(const ring_buffer_t *rb)
{
    (void)rb;
    return false;
}

bool rb_is_full(const ring_buffer_t *rb)
{
    (void)rb;
    return false;
}

void rb_clear(ring_buffer_t *rb)
{
    (void)rb;
}

size_t rb_write(ring_buffer_t *rb, const uint8_t *data, size_t len)
{
    (void)rb;
    (void)data;
    (void)len;
    return 0;
}

size_t rb_read(ring_buffer_t *rb, uint8_t *dst, size_t len)
{
    (void)rb;
    (void)dst;
    (void)len;
    return 0;
}
