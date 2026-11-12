#include "ring_buffer.h"

/* Move an index one step forward, wrapping from size - 1 back to 0. */
static size_t next_index(const ring_buffer_t *rb, size_t index)
{
    index++;
    if (index == rb->size) {
        index = 0u;
    }
    return index;
}

/* Internal one-byte push/pop, used by both the single-byte and the bulk
 * functions. The caller has already checked the pointers. */
static rb_status_t push_byte(ring_buffer_t *rb, uint8_t byte)
{
    /* `count` tells full from empty: head == tail alone is ambiguous. */
    if (rb->count == rb->size) {
        return RB_ERR_FULL;
    }
    rb->buf[rb->head] = byte;
    rb->head = next_index(rb, rb->head);
    rb->count++;
    return RB_OK;
}

static rb_status_t pop_byte(ring_buffer_t *rb, uint8_t *out)
{
    if (rb->count == 0u) {
        return RB_ERR_EMPTY; /* *out is not touched */
    }
    *out = rb->buf[rb->tail];
    rb->tail = next_index(rb, rb->tail);
    rb->count--;
    return RB_OK;
}

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
    return push_byte(rb, byte);
}

rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL) {
        return RB_ERR_NULL;
    }
    return pop_byte(rb, out);
}

rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *out)
{
    if (rb == NULL || out == NULL) {
        return RB_ERR_NULL;
    }
    if (rb->count == 0u) {
        return RB_ERR_EMPTY;
    }
    *out = rb->buf[rb->tail]; /* same as pop, but tail and count stay */
    return RB_OK;
}

size_t rb_count(const ring_buffer_t *rb)
{
    return (rb == NULL) ? 0u : rb->count;
}

size_t rb_free(const ring_buffer_t *rb)
{
    return (rb == NULL) ? 0u : rb->size - rb->count;
}

bool rb_is_empty(const ring_buffer_t *rb)
{
    return (rb == NULL) || (rb->count == 0u);
}

bool rb_is_full(const ring_buffer_t *rb)
{
    return (rb != NULL) && (rb->count == rb->size);
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
    while (written < len && push_byte(rb, data[written]) == RB_OK) {
        written++;
    }
    return written;
}

size_t rb_read(ring_buffer_t *rb, uint8_t *dst, size_t len)
{
    size_t read = 0u;

    if (rb == NULL || dst == NULL) {
        return 0u;
    }
    while (read < len && pop_byte(rb, &dst[read]) == RB_OK) {
        read++;
    }
    return read;
}