#include "pool.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdalign.h>

static alignas(max_align_t) unsigned char storage[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];
static bool used[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        used[i] = false;
    }
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!used[i]) {
            used[i] = true;
            return &storage[i * POOL_BLOCK_SIZE];
        }
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
    uintptr_t total_size = POOL_BLOCK_SIZE * POOL_BLOCK_COUNT;

    if (addr < start || addr >= start + total_size) {
        return false;
    }

    uintptr_t offset = addr - start;

    if (offset % POOL_BLOCK_SIZE != 0) {
        return false;
    }

    size_t index = offset / POOL_BLOCK_SIZE;

    if (!used[index]) {
        return false;
    }

    used[index] = false;
    return true;
}

size_t pool_free_count(void)
{
    size_t count = 0;

    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!used[i]) {
            count++;
        }
    }

    return count;
}
