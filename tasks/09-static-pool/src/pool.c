#include "pool.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    _Alignas(max_align_t) uint8_t data[POOL_BLOCK_SIZE];
} pool_block_t;

static pool_block_t s_pool[POOL_BLOCK_COUNT];
static bool s_used[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        s_used[i] = false;
    }
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!s_used[i]) {
            s_used[i] = true;
            return &s_pool[i].data[0];
        }
    }
    return NULL;
}

bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }
    uintptr_t block_addr = (uintptr_t)block;
    uintptr_t pool_start = (uintptr_t)&s_pool[0];
    uintptr_t pool_end = (uintptr_t)&s_pool[POOL_BLOCK_COUNT];
    if (block_addr < pool_start || block_addr >= pool_end) {
        return false;
    }
    uintptr_t offset = block_addr - pool_start;
    if (offset % sizeof(pool_block_t) != 0) {
        return false;
    }
    size_t index = offset / sizeof(pool_block_t);
    if (!s_used[index]) {
        return false;
    }
    s_used[index] = false;
    return true;
}

size_t pool_free_count(void)
{
    size_t count = 0;
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!s_used[i]) {
            count++;
        }
    }
    return count;
}