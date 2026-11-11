
#include "pool.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdalign.h>

static alignas(max_align_t) uint8_t memory[POOL_BLOCK_SIZE * POOL_BLOCK_COUNT];

static bool in_use[POOL_BLOCK_COUNT];

void pool_init(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        in_use[i] = false;
    }
}

void *pool_alloc(void)
{
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!in_use[i]) {
            in_use[i] = true;
            return &memory[i * POOL_BLOCK_SIZE];
        }
    }
    return NULL;
}

bool pool_free(void *block)
{
    if (block == NULL) {
        return false;
    }

    uint8_t *ptr = (uint8_t *)block;
    ptrdiff_t offset = ptr - memory;

    if (offset < 0 || (size_t)offset >= sizeof(memory) || (size_t)offset % POOL_BLOCK_SIZE != 0) {
        return false;
    }

    size_t index = (size_t)offset / POOL_BLOCK_SIZE;

    if (!in_use[index]) {
        return false;
    }

    in_use[index] = false;
    return true;
}

size_t pool_free_count(void)
{
    size_t count = 0;
    for (size_t i = 0; i < POOL_BLOCK_COUNT; i++) {
        if (!in_use[i]) {
            count++;
        }
    }
    return count;
}