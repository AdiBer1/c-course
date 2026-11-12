#include "pool.h"

#include <stdint.h>
static _Alignas(max_align_t) unsigned char storage[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
/* TODO: declare the static storage and bookkeeping, then implement. */
static bool used[POOL_BLOCK_COUNT];
void pool_init(void) {
    
    size_t i = 0;
    while (i < POOL_BLOCK_COUNT) {
        used[i] = false;
        i++;
    }
}

void *pool_alloc(void)
{
    size_t i = 0;
    while (i < POOL_BLOCK_COUNT) {
        if (!used[i]) {
            used[i] = true;
            return storage[i];
        }
        i++;
    }
    return NULL;
}

bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }

    uintptr_t start = (uintptr_t)storage;
    uintptr_t addr = (uintptr_t)block;
    uintptr_t total = (uintptr_t)POOL_BLOCK_COUNT * POOL_BLOCK_SIZE;

    if (addr < start || addr >= start + total) {
        return false;
    }

    uintptr_t offset = addr - start;
    if (offset % POOL_BLOCK_SIZE != 0) {
        return false;
    }

    size_t index = (size_t)(offset / POOL_BLOCK_SIZE);
    if (!used[index]) {
        return false;
    }

    used[index] = false;
    return true;
}

size_t pool_free_count(void)
{
     size_t count = 0;
    size_t i = 0;
    while (i < POOL_BLOCK_COUNT) {
        if (!used[i]) {
            count++;
        }
        i++;
    }
    return count;
}
