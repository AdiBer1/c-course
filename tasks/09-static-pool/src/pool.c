#include "pool.h"

#include <stddef.h>
#include <stdint.h>

typedef union {
    max_align_t align; /* forces alignment for any type */
    uint8_t bytes[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];
} pool_storage_t;

static pool_storage_t storage;
static bool used[POOL_BLOCK_COUNT];

void pool_init(void)
{
    size_t i;

    for (i = 0; i < POOL_BLOCK_COUNT; i++) {
        used[i] = false;
    }
}

void *pool_alloc(void)
{
    size_t i;

    for (i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!used[i]) {
            used[i] = true;
            return &storage.bytes[i * POOL_BLOCK_SIZE];
        }
    }
    return NULL;
}

bool pool_free(void *block)
{
    uintptr_t addr;
    uintptr_t base;
    size_t offset;
    size_t index;

    if (block == NULL) {
        return false;
    }

    addr = (uintptr_t)block;
    base = (uintptr_t)&storage.bytes[0];

    if (addr < base) {
        return false;
    }
    offset = (size_t)(addr - base);
    if (offset >= (size_t)POOL_BLOCK_SIZE * POOL_BLOCK_COUNT) {
        return false;
    }
    if ((offset % POOL_BLOCK_SIZE) != 0u) {
        return false;
    }

    index = offset / POOL_BLOCK_SIZE;
    if (!used[index]) {
        return false;
    }
    used[index] = false;
    return true;
}

size_t pool_free_count(void)
{
    size_t i;
    size_t count = 0;

    for (i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!used[i]) {
            count++;
        }
    }
    return count;
}