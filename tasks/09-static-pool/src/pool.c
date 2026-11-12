#include "pool.h"

#include <stdint.h>
#include <stdalign.h>

/* TODO: declare the static storage and bookkeeping, then implement. */

alignas(max_align_t) static uint8_t pool_storage[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];
static bool block_allocated[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        block_allocated[i] = false;
    }
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!block_allocated[i]) {
            block_allocated[i] = true;
            return &pool_storage[i * POOL_BLOCK_SIZE];
        }
    }
    return NULL;
}

bool pool_free(void *block)
{
    (void)block;
    return false;
}

size_t pool_free_count(void)
{
    size_t count = 0;
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!block_allocated[i]) {
            count++;
        }
    }
    return count;
}