#include "pool.h"

#include <stdint.h>

typedef union {
    uint8_t bytes[POOL_BLOCK_SIZE];
    long long ll;
    long double ld;
    void *ptr;
} pool_block_t;

static pool_block_t pool_storage[POOL_BLOCK_COUNT];
static bool block_used[POOL_BLOCK_COUNT];
static size_t free_blocks = POOL_BLOCK_COUNT;

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        block_used[i] = false;
    }
    free_blocks = POOL_BLOCK_COUNT;
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!block_used[i]) {
            block_used[i] = true;
            free_blocks--;
            return &pool_storage[i].bytes[0];
        }
    }
    return NULL;
}

bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }

    uintptr_t addr = (uintptr_t)block;
    uintptr_t base = (uintptr_t)&pool_storage[0].bytes[0];
    uintptr_t end = base + sizeof(pool_storage);

    if (addr < base || addr >= end) {
        return false;
    }

    uintptr_t offset = addr - base;
    if ((offset % POOL_BLOCK_SIZE) != 0) {
        return false;
    }

    size_t index = (size_t)(offset / POOL_BLOCK_SIZE);
    if (!block_used[index]) {
        return false;
    }

    block_used[index] = false;
    free_blocks++;
    return true;
}

size_t pool_free_count(void)
{
    return free_blocks;
}