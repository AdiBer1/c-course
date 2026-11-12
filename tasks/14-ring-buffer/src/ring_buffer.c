#include "ring_buffer.h"

rb_status_t rb_init(ring_buffer_t *rb, uint8_t *storage, size_t size)
{
     if ( rb==NULL || storage == NULL )
       return RB_ERR_NULL;
    if ( size ==0 )
       return RB_ERR_SIZE;
    rb->buf = storage;
    rb->size = size;
    rb->head = 0;  
    rb->tail = 0;
    rb->count =0;
    return RB_OK;
}

rb_status_t rb_push(ring_buffer_t *rb, uint8_t byte)
{
     if ( rb == NULL )
      return RB_ERR_NULL; 
    if ( rb->count == rb -> size )
       return RB_ERR_FULL;
    rb->buf[rb->head] = byte;
    rb->count++;
    rb->head++;
    if ( rb->head == rb->size)
       rb->head = 0;
    return RB_OK;
}

rb_status_t rb_pop(ring_buffer_t *rb, uint8_t *out)
{
    if ( rb == NULL || out == NULL )
      return RB_ERR_NULL;
    if( rb->count == 0 )
      return RB_ERR_EMPTY;
    *out = rb->buf[ rb->tail ];
    rb->tail++;
    if ( rb->tail == rb->size ){
       rb->tail = 0;
    }
    rb->count--;
    return RB_OK;

}

rb_status_t rb_peek(const ring_buffer_t *rb, uint8_t *out)
{
    if ( rb == NULL || out == NULL )
      return RB_ERR_NULL;
    if( rb->count == 0 )
      return RB_ERR_EMPTY;
    *out = rb->buf[ rb->tail ];
    return RB_OK;
}

size_t rb_count(const ring_buffer_t *rb)
{
        if (rb == NULL )
      return 0;
    return rb->count;

}

size_t rb_free(const ring_buffer_t *rb)
{
    if (rb == NULL )
      return 0;
    return rb->size - rb->count;
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
