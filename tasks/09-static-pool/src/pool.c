#include "pool.h"

#include <stdint.h>

static _Alignas(max_align_t) unsigned char storage[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];

static bool in_use[POOL_BLOCK_COUNT];

static size_t free_blocks;

_Static_assert(POOL_BLOCK_SIZE % _Alignof(max_align_t) == 0u,
               "block size must keep every block aligned");

void pool_init(void)
{
    for (size_t i = 0u; i < POOL_BLOCK_COUNT; i++) {
        in_use[i] = false;
    }
    free_blocks = POOL_BLOCK_COUNT;
}

void *pool_alloc(void)
{
    for (size_t i = 0u; i < POOL_BLOCK_COUNT; i++) {
        if (!in_use[i]) {
            in_use[i] = true;
            free_blocks--;
            return storage[i];
        }
    }
    return NULL;
}

bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }

    uintptr_t start = (uintptr_t)&storage[0][0];
    uintptr_t addr = (uintptr_t)block;
    uintptr_t total = (uintptr_t)POOL_BLOCK_COUNT * POOL_BLOCK_SIZE;

    if (addr < start || addr >= start + total) {
        return false;
    }
    uintptr_t offset = addr - start;
    if (offset % POOL_BLOCK_SIZE != 0u) {
        return false;
    }

    size_t index = (size_t)(offset / POOL_BLOCK_SIZE);
    if (!in_use[index]) {
        return false;
    }

    in_use[index] = false;
    free_blocks++;
    return true;
}

size_t pool_free_count(void)
{
    return free_blocks;
}
